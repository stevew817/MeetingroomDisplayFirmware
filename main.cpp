/*
 * Copyright (c) 2015, 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <string>
#include <sstream>
#include <vector>
#include "mbed-trace/mbed_trace.h"
#include "mbedtls/entropy_poll.h"

#include "security.h"
#include "ntp-client/NTPClient.h"
#include "Adafruit-GFX-Library/LS027B7DH01.h"

#include "SansSerifPlain_60.h"
#include "SansSerifPlain_48.h"
#include "SansSerifPlain_36.h"
#include "SansSerifPlain_24.h"

#include "mbed.h"

/* Inlined dependency on easy-connect.
 * Might want to re-depend on easy-connect once EFR32 support makes it in. */
#define MESH
#include "NanostackInterface.h"
ThreadInterface mesh;
#include "NanostackRfPhyEfr32.h"
NanostackRfPhyEfr32 rf_phy;
#define MBED_SERVER_ADDRESS "coaps://[2607:f0d0:2601:52::20]:5684"
#include "simpleclient.h"

#define RED_LED (LED1)
#define GREEN_LED (LED2)		     
#define LED_ON (0)
#define LED_OFF (!LED_ON)

// Status indication
DigitalOut red_led(RED_LED);
DigitalOut green_led(GREEN_LED);

Ticker status_ticker;
void blinky() {
    green_led = !green_led;
}

// These are example resource values for the Device Object
struct MbedClientDevice device = {
    "Stevew817",        // Manufacturer
    "ConfrmMonitor",    // Type
    "A00",              // ModelNumber
    ""                  // SerialNumber will be filled out later with MAC address
};

// Instantiate the class which implements LWM2M Client API (from simpleclient.h)
MbedClient mbed_client;

// Keep the mbed client example's button object
InterruptIn obs_button(SW1);

// Network interaction must be performed outside of interrupt context
volatile bool registered = false;
volatile bool clicked = false;
volatile bool new_data = false;
osThreadId mainThread;

typedef enum {
    TIME_UPDATE,
    ROOM_UPDATE,
    CUR_OWNER_UPDATE,
    CUR_TIME_UPDATE,
    NEXT_OWNER_UPDATE,
    NEXT_TIME_UPDATE,
    BUTTON_CLICKED,
    UNREGISTER
} ProgramEventType_t;

typedef struct {
    ProgramEventType_t event;
    void* data;
} ProgramEvent_t;

Queue<ProgramEvent_t, 16> evqueue;

void unregister() {
    ProgramEvent_t* event = (ProgramEvent_t*) malloc(sizeof(ProgramEvent_t));
    if (event == NULL) return;
    event->event = UNREGISTER;
    evqueue.put(event);
}

void button_clicked() {
    ProgramEvent_t* event = (ProgramEvent_t*) malloc(sizeof(ProgramEvent_t));
    if (event == NULL) return;
    event->event = BUTTON_CLICKED;
    evqueue.put(event);
}

/*
 * Arguments for running "blink" in it's own thread.
 */
class BlinkArgs {
public:
    BlinkArgs() {
        clear();
    }
    void clear() {
        position = 0;
        blink_pattern.clear();
    }
    uint16_t position;
    std::vector<uint32_t> blink_pattern;
};

/*
 * The Led contains one property (pattern) and a function (blink).
 * When the function blink is executed, the pattern is read, and the LED
 * will blink based on the pattern.
 */
class LedResource {
public:
    LedResource() {
        // create ObjectID with metadata tag of '3201', which is 'digital output'
        led_object = M2MInterfaceFactory::create_object("3201");
        M2MObjectInstance* led_inst = led_object->create_object_instance();

        // 5853 = Multi-state output
        M2MResource* pattern_res = led_inst->create_dynamic_resource("5853", "Pattern",
            M2MResourceInstance::STRING, false);
        // read and write
        pattern_res->set_operation(M2MBase::GET_PUT_ALLOWED);
        // set initial pattern (toggle every 200ms. 7 toggles in total)
        pattern_res->set_value((const uint8_t*)"500:500:500:500:500:500:500", 27);

        // there's not really an execute LWM2M ID that matches... hmm...
        M2MResource* led_res = led_inst->create_dynamic_resource("5850", "Blink",
            M2MResourceInstance::OPAQUE, false);
        // we allow executing a function here...
        led_res->set_operation(M2MBase::POST_ALLOWED);
        // when a POST comes in, we want to execute the led_execute_callback
        led_res->set_execute_function(execute_callback(this, &LedResource::blink));
        // Completion of execute function can take a time, that's why delayed response is used
        led_res->set_delayed_response(true);
        blink_args = new BlinkArgs();
    }

    ~LedResource() {
        delete blink_args;
    }

    M2MObject* get_object() {
        return led_object;
    }

    int mem_atoi(const char *buf, size_t len)              // spaces, sign, digits
    {
            int n=0, sign=1;

            while ( len && isspace(*buf) )
                    --len, ++buf;

            if ( len ) switch(*buf) {
                    case '-':       sign=-1;        \
                    case '+':       --len, ++buf;
            }

            while ( len-- && isdigit(*buf) )
                    n = n*10 + *buf++ -'0';

            return n*sign;
    }

    void blink(void *argument) {
        // read the value of 'Pattern'
        status_ticker.detach();
        green_led = LED_OFF;

        M2MObjectInstance* inst = led_object->object_instance();
        M2MResource* res = inst->resource("5853");
        // Clear previous blink data
        blink_args->clear();

        // values in mbed Client are all buffers, and we need a vector of int's
        uint8_t* buffIn = res->value();
        uint32_t sizeIn = res->value_length();

        // turn the buffer into a string, and initialize a vector<int> on the heap
        printf("led_execute_callback pattern=%s\n", buffIn);

        // our pattern is something like 500:200:500, so parse that
        // Ripped out std::String usage since it was pulling in 15k of bloat from stdlibc++
        size_t start = 0;
        size_t index = 0;
        while(index < sizeIn) {
            if(buffIn[index] == ':') {
                if(index == 0) {
                    start = 1;
                } else {
                    blink_args->blink_pattern.push_back(mem_atoi((const char*)&buffIn[start], index - start - 1));
                    start = index + 1;
                }
            }
            index++;
        }
        if (start < index) {
            blink_args->blink_pattern.push_back(mem_atoi((const char*)&buffIn[start], index - start - 1));
        }

        // check if POST contains payload
        if (argument) {
            M2MResource::M2MExecuteParameter* param = (M2MResource::M2MExecuteParameter*)argument;
            String object_name = param->get_argument_object_name();
            uint16_t object_instance_id = param->get_argument_object_instance_id();
            String resource_name = param->get_argument_resource_name();
            int payload_length = param->get_argument_value_length();
            const uint8_t* payload = param->get_argument_value();
            printf("Resource: %s/%d/%s executed\n", object_name.c_str(), object_instance_id, resource_name.c_str());
            printf("Payload: %.*s\n", payload_length, payload);
        }
        // do_blink is called with the vector, and starting at -1
        blinky_thread.start(callback(this, &LedResource::do_blink));
    }

private:
    M2MObject* led_object;
    Thread blinky_thread;
    BlinkArgs *blink_args;
    void do_blink() {
        for (;;) {
            // blink the LED
            red_led = !red_led;
            // up the position, if we reached the end of the vector
            if (blink_args->position >= blink_args->blink_pattern.size()) {
                // send delayed response after blink is done
                M2MObjectInstance* inst = led_object->object_instance();
                M2MResource* led_res = inst->resource("5850");
                led_res->send_delayed_post_response();
                red_led = LED_OFF;
                status_ticker.attach_us(blinky, 250000);
                return;
            }
            // Wait requested time, then continue prosessing the blink pattern from next position.
            Thread::wait(blink_args->blink_pattern.at(blink_args->position));
            blink_args->position++;
        }
    }
};

/*
 * The button contains one property (click count).
 * When `handle_button_click` is executed, the counter updates.
 */
class ButtonResource {
public:
    ButtonResource(): counter(0) {
        // create ObjectID with metadata tag of '3200', which is 'digital input'
        btn_object = M2MInterfaceFactory::create_object("3200");
        M2MObjectInstance* btn_inst = btn_object->create_object_instance();
        // create resource with ID '5501', which is digital input counter
        M2MResource* btn_res = btn_inst->create_dynamic_resource("5501", "Button",
            M2MResourceInstance::INTEGER, true /* observable */);
        // we can read this value
        btn_res->set_operation(M2MBase::GET_ALLOWED);
        // set initial value (all values in mbed Client are buffers)
        // to be able to read this data easily in the Connector console, we'll use a string
        btn_res->set_value((uint8_t*)"0", 1);
    }

    ~ButtonResource() {
    }

    M2MObject* get_object() {
        return btn_object;
    }

    /*
     * When you press the button, we read the current value of the click counter
     * from mbed Device Connector, then up the value with one.
     */
    void handle_button_click() {
        if (mbed_client.register_successful()) {
            M2MObjectInstance* inst = btn_object->object_instance();
            M2MResource* res = inst->resource("5501");

            // up counter
            counter++;
    #ifdef TARGET_K64F
            printf("handle_button_click, new value of counter is %d\n", counter);
    #else
            printf("simulate button_click, new value of counter is %d\n", counter);
    #endif
            // serialize the value of counter as a string, and tell connector
            char buffer[20];
            int size = sprintf(buffer,"%d",counter);
            res->set_value((uint8_t*)buffer, size);
        } else {
            printf("simulate button_click, device not registered\n");
        }
    }

private:
    M2MObject* btn_object;
    uint16_t counter;
};

/*
 * The Meeting Room Monitor contains properties and a refresh function.
 * When the function is executed, we'll redraw the memoryLCD. This way, we don't
 * need to do it on every property update
 */
 
mbed::DigitalOut CS(PC9);
mbed::DigitalOut EXTCOM(PC7);
 
mbed::SPI displaySPI(PC6, NC, PC8);
silabs::LS027B7DH01 display(&displaySPI, &CS, &EXTCOM);

class MeetingRoomMonitorResource {
public:
    MeetingRoomMonitorResource() {
        // create ObjectID with custom metadata tag of '32111'
        meetingroommonitor_obj = M2MInterfaceFactory::create_object("32111");
        meetingroommonitor_inst = meetingroommonitor_obj->create_object_instance();

        // 5527 = Text
        current_owner_res = meetingroommonitor_inst->create_dynamic_resource("32100", "CurrentOwner",
            M2MResourceInstance::STRING, true);
        // read and write
        current_owner_res->set_operation(M2MBase::GET_PUT_ALLOWED);
        // set initial owner
        current_owner_res->set_value((const uint8_t*)"Free", 5);
        current_owner_res->set_value_updated_function(value_updated_callback(this, &MeetingRoomMonitorResource::update_current_owner));
        
        current_slot_res = meetingroommonitor_inst->create_dynamic_resource("32101", "CurrentSlot",
            M2MResourceInstance::STRING, true);
        // read and write
        current_slot_res->set_operation(M2MBase::GET_PUT_ALLOWED);
        // set initial owner
        current_slot_res->set_value((const uint8_t*)"00:00 - 00:00", 14);
        current_slot_res->set_value_updated_function(value_updated_callback(this, &MeetingRoomMonitorResource::update_current_slot));
        
        next_owner_res = meetingroommonitor_inst->create_dynamic_resource("32102", "NextOwner",
            M2MResourceInstance::STRING, true);
        // read and write
        next_owner_res->set_operation(M2MBase::GET_PUT_ALLOWED);
        // set initial owner
        next_owner_res->set_value((const uint8_t*)"Free", 5);
        next_owner_res->set_value_updated_function(value_updated_callback(this, &MeetingRoomMonitorResource::update_next_owner));
        
        next_slot_res = meetingroommonitor_inst->create_dynamic_resource("32103", "NextSlot",
            M2MResourceInstance::STRING, true);
        // read and write
        next_slot_res->set_operation(M2MBase::GET_PUT_ALLOWED);
        // set initial owner
        next_slot_res->set_value((const uint8_t*)"00:00 - 00:00", 14);
        next_slot_res->set_value_updated_function(value_updated_callback(this, &MeetingRoomMonitorResource::update_next_slot));
        
        roomname_res = meetingroommonitor_inst->create_dynamic_resource("32104", "RoomName",
            M2MResourceInstance::STRING, true);
        // read and write
        roomname_res->set_operation(M2MBase::GET_PUT_ALLOWED);
        // set initial owner
        roomname_res->set_value((const uint8_t*)"Uninit", 7);
        roomname_res->set_value_updated_function(value_updated_callback(this, &MeetingRoomMonitorResource::update_roomname));

        // there's not really an execute LWM2M ID that matches... hmm...
        M2MResource* exec_res = meetingroommonitor_inst->create_dynamic_resource("5850", "Update",
            M2MResourceInstance::OPAQUE, false);
        // we allow executing a function here...
        exec_res->set_operation(M2MBase::POST_ALLOWED);
        // when a POST comes in, we want to execute the led_execute_callback
        // exec_res->set_execute_function(execute_callback(this, &MeetingRoomMonitorResource::newdata));
    }

    ~MeetingRoomMonitorResource() {
    }

    M2MObject* get_object() {
        return meetingroommonitor_obj;
    }

    M2MObjectInstance* get_instance() {
        return meetingroommonitor_inst;
    }

    void update_current_owner(const char* argument) {
        ProgramEvent_t* event = (ProgramEvent_t*) malloc(sizeof(ProgramEvent_t));
        if (event == NULL) return;

        size_t length = current_owner_res->value_length() + 1;
        void* dataPtr = malloc(length);
        if (dataPtr == NULL) {
            free((void*)event);
            return;
        }

        memcpy(dataPtr, current_owner_res->value(), length);
        ((char*)dataPtr)[length-1] = 0x00;

        event->event = CUR_OWNER_UPDATE;
        event->data = dataPtr;
        evqueue.put(event);
    }

    void update_current_slot(const char* argument) {
        ProgramEvent_t* event = (ProgramEvent_t*) malloc(sizeof(ProgramEvent_t));
        if (event == NULL) return;

        size_t length = current_slot_res->value_length() + 1;
        void* dataPtr = malloc(length);
        if (dataPtr == NULL) {
            free((void*)event);
            return;
        }

        memcpy(dataPtr, current_slot_res->value(), length);
        ((char*)dataPtr)[length-1] = 0x00;

        event->event = CUR_TIME_UPDATE;
        event->data = dataPtr;
        evqueue.put(event);
    }

    void update_next_slot(const char* argument) {
        ProgramEvent_t* event = (ProgramEvent_t*) malloc(sizeof(ProgramEvent_t));
        if (event == NULL) return;

        size_t length = next_slot_res->value_length() + 1;
        void* dataPtr = malloc(length);
        if (dataPtr == NULL) {
            free((void*)event);
            return;
        }

        memcpy(dataPtr, next_slot_res->value(), length);
        ((char*)dataPtr)[length-1] = 0x00;

        event->event = NEXT_TIME_UPDATE;
        event->data = dataPtr;
        evqueue.put(event);
    }

    void update_next_owner(const char* argument) {
        ProgramEvent_t* event = (ProgramEvent_t*) malloc(sizeof(ProgramEvent_t));
        if (event == NULL) return;

        size_t length = next_owner_res->value_length() + 1;
        void* dataPtr = malloc(length);
        if (dataPtr == NULL) {
            free((void*)event);
            return;
        }

        memcpy(dataPtr, next_owner_res->value(), length);
        ((char*)dataPtr)[length-1] = 0x00;

        event->event = NEXT_OWNER_UPDATE;
        event->data = dataPtr;
        evqueue.put(event);
    }

    void update_roomname(const char* argument) {
        ProgramEvent_t* event = (ProgramEvent_t*) malloc(sizeof(ProgramEvent_t));
        if (event == NULL) return;

        size_t length = roomname_res->value_length() + 1;
        void* dataPtr = malloc(length);
        if (dataPtr == NULL) {
            free((void*)event);
            return;
        }

        memcpy(dataPtr, roomname_res->value(), length);
        ((char*)dataPtr)[length-1] = 0x00;

        event->event = ROOM_UPDATE;
        event->data = dataPtr;
        evqueue.put(event);
    }

private:
    M2MObject* meetingroommonitor_obj;
    M2MObjectInstance* meetingroommonitor_inst;

    M2MResource* current_owner_res;
    M2MResource* current_slot_res;
    M2MResource* next_owner_res;
    M2MResource* next_slot_res;
    M2MResource* roomname_res;
};

// Entry point to the program
int main() {

    unsigned int seed;
    size_t len;

#ifdef MBEDTLS_ENTROPY_HARDWARE_ALT
    // Used to randomize source port
    mbedtls_hardware_poll(NULL, (unsigned char *) &seed, sizeof seed, &len);

#elif defined MBEDTLS_TEST_NULL_ENTROPY

#warning "mbedTLS security feature is disabled. Connection will not be secure !! Implement proper hardware entropy for your selected hardware."
    // Used to randomize source port
    mbedtls_null_entropy_poll( NULL,(unsigned char *) &seed, sizeof seed, &len);

#else

#error "This hardware does not have entropy, endpoint will not register to Connector.\
You need to enable NULL ENTROPY for your application, but if this configuration change is made then no security is offered by mbed TLS.\
Add MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES and MBEDTLS_TEST_NULL_ENTROPY in mbed_app.json macros to register your endpoint."

#endif

    srand(seed);
    red_led = LED_OFF;
    
    displaySPI.frequency(1000000);
    display.clearImmediate();

    status_ticker.attach_us(blinky, 250000);
    // Keep track of the main thread
    mainThread = osThreadGetId();

    printf("\nStarting meeting room display in IPv6 mode (Thread)\n");

    mbed_trace_init();

    NetworkInterface* network = &mesh;
    mesh.initialize(&rf_phy);

    int connect_success = -1;
    connect_success = mesh.connect();

    if(connect_success < 0) {
        printf("\nConnection to Network Failed - exiting application...\n");
        return -1;
    } else {
        const char *ip_addr  = mesh.get_ip_address();
        printf("IP address %s\n", ip_addr);
    }
    
    // Try to get current time. If successful, we have internet connectivity
    NTPClient ntp(network);
    time_t ts = ntp.get_timestamp();
    
    while(ts <= 0) {
        ts = ntp.get_timestamp();
        
        if (ts < 0) {
            printf("An error occurred when getting the time. Code: %ld\r\n", ts);
        }
        printf("Waiting 10 seconds before trying again...\r\n");
        wait(10.0);        
    }
    printf("Current time is %s\r\n", ctime(&ts));
    set_time(ts);

    // we create our LWM2M resources
    ButtonResource button_resource;
    LedResource led_resource;
    MeetingRoomMonitorResource meetingroommonitor_resource;

    // Observation Button (SW2) press will send update of endpoint resource values to connector
    obs_button.fall(&button_clicked);

    // Create endpoint interface to manage register and unregister
    mbed_client.create_interface(MBED_SERVER_ADDRESS, network);

    // Get MAC address as serial number
    uint8_t mac[8];
    rf_phy.get_mac_address(mac);
    char macstring[24];
    for(size_t i = 0; i < sizeof(mac); i++) {
        macstring[i*3] = (mac[i] >> 4) > 0x9 ? (mac[i] >> 4) + 0x37 : (mac[i] >> 4) + 0x30;
        macstring[i*3 + 1] = (mac[i] & 0x0F) > 0x9 ? (mac[i] & 0x0F) + 0x37 : (mac[i] & 0x0F) + 0x30;
        if (i < (sizeof(mac) - 1)) {
            macstring[i*3 + 2] = ':';
        }
    }
    device.SerialNumber = (const char*)macstring;

    // Create Objects of varying types, see simpleclient.h for more details on implementation.
    M2MSecurity* register_object = mbed_client.create_register_object(); // server object specifying connector info
    M2MDevice*   device_object   = mbed_client.create_device_object(&device);   // device resources object

    // Create list of Objects to register
    M2MObjectList object_list;

    // Add objects to list
    object_list.push_back(device_object);
    object_list.push_back(button_resource.get_object());
    object_list.push_back(led_resource.get_object());
    object_list.push_back(meetingroommonitor_resource.get_object());

    // Set endpoint registration object
    mbed_client.set_register_object(register_object);

    // Register with mbed Device Connector
    mbed_client.test_register(register_object, object_list);
    registered = true;

    display.fillScreen(0);
    // values in mbed Client are all buffers, and we need a vector of int's
    uint8_t* buffIn = NULL;
    uint32_t sizeIn;
    
    M2MObjectInstance* inst = meetingroommonitor_resource.get_object()->object_instance();
    M2MResource* res = inst->resource("32100");
    display.setFont(&SansSerif_plain_60);
    display.setCursor(200,60);
    
    time_t timestamp= time(NULL);
    struct tm *tmp = gmtime(&timestamp);
    display.printf("%02d:%02d", tmp->tm_hour, tmp->tm_min);
    
    display.setFont(&SansSerif_plain_24);
    display.setCursor(230,84);
    display.printf("%02d/%02d/%02d", tmp->tm_mday, tmp->tm_mon + 1, tmp->tm_year%100);
    
    display.setFont(&SansSerif_plain_24);
    display.setCursor(0,24);
    
    res = inst->resource("32104");
    buffIn = res->value();
    sizeIn = res->value_length();
    if (sizeIn > 0) {
        display.printf("%s", buffIn);
    }
    
    res = inst->resource("32103");
    buffIn = res->value();
    sizeIn = res->value_length();
    display.setCursor(0,72);
    if (sizeIn > 0) {
        display.printf("Next:\n%s", buffIn);
    }
    
    res = inst->resource("32102");
    display.setCursor(0,120);
    buffIn = res->value();
    sizeIn = res->value_length();
    if (sizeIn > 0) {
        display.printf("%s", buffIn);
    }
    
    display.setFont(&SansSerif_plain_36);
    display.setCursor(80,200);
    res = inst->resource("32101");
    buffIn = res->value();
    sizeIn = res->value_length();
    if (sizeIn > 0) {
        display.printf("%s", buffIn);
    }
    
    display.setFont(&SansSerif_plain_36);
    display.setCursor(10,236);
    res = inst->resource("32100");
    buffIn = res->value();
    sizeIn = res->value_length();
    if (sizeIn > 0) {
        display.printf("%s", buffIn);
    }
    
    display.update();

    while (registered) {
        osEvent event = evqueue.get(15000);
        if(event.status == (osStatus)osEventMessage) {
            ProgramEvent_t* evPtr = (ProgramEvent_t*)event.value.p;
            switch(evPtr->event) {
                case TIME_UPDATE:
                    // Update clock
                    display.fillRect(200,0,200,90,0);
                    display.setFont(&SansSerif_plain_60);
                    display.setCursor(200,60);
                    
                    timestamp= time(NULL);
                    tmp = gmtime(&timestamp);
                    display.printf("%02d:%02d", tmp->tm_hour + 2, tmp->tm_min);
                    display.setFont(&SansSerif_plain_24);
                    display.setCursor(230,84);
                    display.printf("%02d/%02d/%02d", tmp->tm_mday, tmp->tm_mon + 1, tmp->tm_year%100);

                    display.update();
                    break;
                case ROOM_UPDATE:
                    display.setFont(&SansSerif_plain_24);
                    display.setCursor(0,30);
                    display.fillRect(0,30-24,200,24,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated room name\n");
                    break;
                case CUR_OWNER_UPDATE:
                    display.setFont(&SansSerif_plain_36);
                    display.setCursor(10,236);
                    display.fillRect(10,236-36,390,36,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated current owner\n");
                    break;
                case CUR_TIME_UPDATE:
                    display.setFont(&SansSerif_plain_36);
                    display.setCursor(80,200);
                    display.fillRect(80,200-36,320,36,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated current slot\n");
                    break;
                case NEXT_OWNER_UPDATE:
                    display.setFont(&SansSerif_plain_24);
                    display.setCursor(0,120);
                    display.fillRect(0,120-24,400,24,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated next owner\n");
                    break;
                case NEXT_TIME_UPDATE:
                    display.setFont(&SansSerif_plain_24);
                    display.setCursor(0,96);
                    display.fillRect(0,96-24,400,24,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated next slot\n");
                    break;
                case BUTTON_CLICKED:
                    button_resource.handle_button_click();
                    break;
                case UNREGISTER:
                    registered = false;
                    break;
                default: break;
            }
            free(evPtr);
        }
        // Update clock
        display.fillRect(200,0,200,90,0);
        display.setFont(&SansSerif_plain_60);
        display.setCursor(200,60);
        
        timestamp= time(NULL);
        tmp = gmtime(&timestamp);
        display.printf("%02d:%02d", tmp->tm_hour + 2, tmp->tm_min);
        display.setFont(&SansSerif_plain_24);
        display.setCursor(230,84);
        display.printf("%02d/%02d/%02d", tmp->tm_mday, tmp->tm_mon + 1, tmp->tm_year%100);

        display.update();
    }

    mbed_client.test_unregister();
    status_ticker.detach();
}
