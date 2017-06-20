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
#include "microSansSerif24.h"
#include "microSansSerif16.h"
#include "microSansSerif12.h"

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

#define GREEN_LED (LED2)		     
#define LED_ON (0)
#define LED_OFF (!LED_ON)

// Status indication
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
static MbedClient mbed_client;
static M2MDevice* device_object;

// Network interaction must be performed outside of interrupt context
volatile bool registered = false;
volatile bool clicked = false;
osThreadId mainThread;

// Offset from UTC in seconds
static volatile int utcOffset = 0;

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

void update_timezone(const char *argument) {
    String tz = device_object->resource_value_string(M2MDevice::UTCOffset);
    //Parse offset
    if (tz == NULL) {
        utcOffset = 0;
    } else if (tz.length() >= 3) {
        // Format +00, +0100 or +01:00
        utcOffset = (tz[1] - 0x30) * 10 * 60 * 60;
        utcOffset += (tz[2] - 0x30) * 60 * 60;

        if (tz.length() <= 5) {
            utcOffset += (tz[3] - 0x30) * 30 * 60;
            utcOffset += (tz[4] - 0x30) * 60;
        } else {
            utcOffset += (tz[4] - 0x30) * 30 * 60;
            utcOffset += (tz[5] - 0x30) * 60;
        }
        if (tz[0] == '-') {
            utcOffset *= -1;
        }
    }
}

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
        char* dataPtr = (char*)malloc(length);
        if (dataPtr == NULL) {
            free((void*)event);
            return;
        }

        memcpy(dataPtr, current_owner_res->value(), length);

        for(size_t i = 0;i < length; i++) {
            // Convert specific UTF8 into character point (up to U+00FF)
            if(dataPtr[i] == 0xC2) {
                length--;
                for(size_t j = i; j < length; j++) {
                    dataPtr[j] = dataPtr[j+1];
                }
            } else if (dataPtr[i] == 0xC3) {
                length--;
                dataPtr[i] = dataPtr[i+1] + 0x40;
                for(size_t j = i+1; j < length; j++) {
                    dataPtr[j] = dataPtr[j+1];
                }
            }
        }
        
        dataPtr[length-1] = 0x00;

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
        char* dataPtr = (char*)malloc(length);
        if (dataPtr == NULL) {
            free((void*)event);
            return;
        }

        memcpy(dataPtr, next_owner_res->value(), length);

        for(size_t i = 0;i < length; i++) {
            // Convert specific UTF8 into character point (up to U+00FF)
            if(dataPtr[i] == 0xC2) {
                length--;
                for(size_t j = i; j < length; j++) {
                    dataPtr[j] = dataPtr[j+1];
                }
            } else if (dataPtr[i] == 0xC3) {
                length--;
                dataPtr[i] = dataPtr[i+1] + 0x40;
                for(size_t j = i+1; j < length; j++) {
                    dataPtr[j] = dataPtr[j+1];
                }
            }
        }
        dataPtr[length-1] = 0x00;

        event->event = NEXT_OWNER_UPDATE;
        event->data = dataPtr;
        evqueue.put(event);
    }

    void update_roomname(const char* argument) {
        ProgramEvent_t* event = (ProgramEvent_t*) malloc(sizeof(ProgramEvent_t));
        if (event == NULL) return;

        size_t length = roomname_res->value_length() + 1;
        char* dataPtr = (char*)malloc(length);
        if (dataPtr == NULL) {
            free((void*)event);
            return;
        }

        memcpy(dataPtr, roomname_res->value(), length);

        for(size_t i = 0;i < length; i++) {
            // Convert specific UTF8 into character point (up to U+00FF)
            if(dataPtr[i] == 0xC2) {
                length--;
                for(size_t j = i; j < length; j++) {
                    dataPtr[j] = dataPtr[j+1];
                }
            } else if (dataPtr[i] == 0xC3) {
                length--;
                dataPtr[i] = dataPtr[i+1] + 0x40;
                for(size_t j = i+1; j < length; j++) {
                    dataPtr[j] = dataPtr[j+1];
                }
            }
        }

        dataPtr[length-1] = 0x00;

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
    
    displaySPI.frequency(5000000);
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
    MeetingRoomMonitorResource meetingroommonitor_resource;

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
    device_object   = mbed_client.create_device_object(&device);   // device resources object
    M2MResource* offsetresource = device_object->create_resource(M2MDevice::UTCOffset, "+00"); // Default to UTC
    offsetresource->set_value_updated_function(update_timezone);

    // Create list of Objects to register
    M2MObjectList object_list;

    // Add objects to list
    object_list.push_back(device_object);
    object_list.push_back(meetingroommonitor_resource.get_object());

    // Set endpoint registration object
    mbed_client.set_register_object(register_object);

    // Register with mbed Device Connector
    mbed_client.test_register(register_object, object_list);
    registered = true;

    // Set initial display state
    display.fillScreen(0);
    display.setFont(&SansSerif_plain_60);
    display.setCursor(220,60);
    
    time_t timestamp= time(NULL);
    timestamp += utcOffset; //apply timezone setting
    struct tm *tmp = gmtime(&timestamp);
    display.printf("%02d:%02d", tmp->tm_hour, tmp->tm_min);
    
    display.setFont(&micross12pt8b);
    display.setCursor(250,84);
    display.printf("%02d/%02d/%02d", tmp->tm_mday, tmp->tm_mon + 1, tmp->tm_year%100);
    
    display.setFont(&micross12pt8b);
    display.setCursor(0,72);
    display.printf("Next:");
    display.update();

    while (registered) {
        osEvent event = evqueue.get(15000);
        if(event.status == (osStatus)osEventMessage) {
            int16_t ulx, uly;
            uint16_t w, h;
            ProgramEvent_t* evPtr = (ProgramEvent_t*)event.value.p;
            switch(evPtr->event) {
                case TIME_UPDATE:
                    // Update clock
                    display.fillRect(220,0,180,90,0);
                    display.setFont(&SansSerif_plain_60);
                    display.setCursor(220,60);
                    
                    timestamp= time(NULL);
                    timestamp += utcOffset; //apply timezone setting
                    tmp = gmtime(&timestamp);
                    display.printf("%02d:%02d", tmp->tm_hour, tmp->tm_min);
                    display.setFont(&micross12pt8b);
                    display.setCursor(250,84);
                    display.printf("%02d/%02d/%02d", tmp->tm_mday, tmp->tm_mon + 1, tmp->tm_year%100);

                    display.update();
                    break;
                case ROOM_UPDATE:
                    display.setFont(&micross16pt8b);
                    display.setCursor(0,30);
                    display.fillRect(0,30-24,200,24,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated room name\n");
                    break;
                case CUR_OWNER_UPDATE:
                    display.setFont(&micross24pt8b);
                    display.getTextBounds((const char*)evPtr->data, 0, 240-11, &ulx, &uly, &w, &h);
                    if(w >= 400) {
                        //String won't fit...
                        ulx = 0;
                    } else {
                        ulx = (400 - w) / 2;
                    }
                    display.setCursor(ulx,240-11);
                    display.fillRect(0,240-54,400,54,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated current owner\n");
                    break;
                case CUR_TIME_UPDATE:
                    display.setFont(&micross24pt8b);
                    display.getTextBounds((const char*)evPtr->data, 0, 240-11, &ulx, &uly, &w, &h);
                    if(w >= 400) {
                        //String won't fit...
                        ulx = 0;
                    } else {
                        ulx = (400 - w) / 2;
                    }
                    display.setCursor(ulx,240-4-54);
                    display.fillRect(0,240-54-40,400,40,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated current slot\n");
                    break;
                case NEXT_OWNER_UPDATE:
                    display.setFont(&micross16pt8b);
                    display.setCursor(0,142-7-6);
                    display.fillRect(0,142-35-6,400,35,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated next owner\n");
                    break;
                case NEXT_TIME_UPDATE:
                    display.setFont(&micross16pt8b);
                    display.setCursor(0,142-7-35);
                    // Numbers: [22,-1]
                    display.fillRect(0,142-35-7-22,400,28,0);
                    display.printf("%s", evPtr->data);
                    free(evPtr->data);
                    display.update();
                    printf("Updated next slot\n");
                    break;
                case BUTTON_CLICKED:
                    break;
                case UNREGISTER:
                    registered = false;
                    break;
                default: break;
            }
            free(evPtr);
        }

        // Update clock
        display.fillRect(220,0,180,90,0);
        display.setFont(&SansSerif_plain_60);
        display.setCursor(220,60);
        
        timestamp = time(NULL);
        timestamp += utcOffset; //apply timezone setting
        tmp = gmtime(&timestamp);
        display.printf("%02d:%02d", tmp->tm_hour, tmp->tm_min);
        display.setFont(&micross12pt8b);
        display.setCursor(250,84);
        display.printf("%02d/%02d/%02d", tmp->tm_mday, tmp->tm_mon + 1, tmp->tm_year%100);
        display.update();

        // Try to sync with NTP every Sunday night at 4AM. This should capture DST changes
        if( tmp->tm_hour == 4
            && tmp->tm_min == 0
            && tmp->tm_wday == 0) {
            ts = ntp.get_timestamp();
            while(ts <= 0) {
                ts = ntp.get_timestamp();
                if (ts < 0) {
                    printf("An error occurred when getting the time. Code: %ld\r\n", ts);
                }
                wait(5.0);
            }
            printf("Current time is %s\r\n", ctime(&ts));
            set_time(ts);
        }
    }

    mbed_client.test_unregister();
    status_ticker.detach();
}
