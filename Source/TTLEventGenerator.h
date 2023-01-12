/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2022 Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROCESSORPLUGIN_H_DEFINED
#define PROCESSORPLUGIN_H_DEFINED

#include <ProcessorHeaders.h>


class TTLEventGenerator : public GenericProcessor
{
public:
    /** The class constructor, used to initialize any members. */
    TTLEventGenerator();
    
    /** The class destructor, used to deallocate memory */
    ~TTLEventGenerator();
    
    /** If the processor has a custom editor, this method must be defined to instantiate it. */
    AudioProcessorEditor* createEditor() override;
    
    /** Called every time the settings of an upstream plugin are changed.
     Allows the processor to handle variations in the channel configuration or any other parameter
     passed through signal chain. The processor can use this function to modify channel objects that
     will be passed to downstream plugins. */
    void updateSettings() override;
    
    /** Defines the functionality of the processor.
     The process method is called every time a new data buffer is available.
     Visualizer plugins typically use this method to send data to the canvas for display purposes */
    
    /** Called whenever a parameter's value is changed */
    void parameterValueChanged(Parameter* param) override;
    
    bool startAcquisition() override;
    
    void process(AudioBuffer<float>& buffer) override;
    
    /** Handles events received by the processor
     Called automatically for each received event whenever checkForEvents() is called from
     the plugin's process() method */
    void handleTTLEvent(TTLEventPtr event) override;
    
    /** Handles spikes received by the processor
     Called automatically for each received spike whenever checkForEvents(true) is called from
     the plugin's process() method */
    void handleSpike(SpikePtr spike) override;
    
    /** Handles broadcast messages sent during acquisition
     Called automatically whenever a broadcast message is sent through the signal chain */
    void handleBroadcastMessage(String message) override;
    
    /** Saving custom settings to XML. This method is not needed to save the state of
     Parameter objects */
    void saveCustomParametersToXml(XmlElement* parentElement) override;
    
    /** Load custom settings from XML. This method is not needed to load the state of
     Parameter objects*/
    void loadCustomParametersFromXml(XmlElement* parentElement) override;
    
private:
    EventChannel* ttlChannel; // local pointer to TTL output channel
    
    int counter = 0; // counts the total number of incoming samples
    bool state = false; // holds the state of the current TTL line (on or off)
    
    bool shouldTriggerEvent = false; // true if an event should be manually triggered
    bool eventWasTriggered = false; // true if an event was manually triggered
    int triggeredEventCounter = 0; // counter for manually triggered events

    float eventIntervalMs = 1000.0f; // time between events
    int outputLine = 0; // TTL output line
};

#endif
