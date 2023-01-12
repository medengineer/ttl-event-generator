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

#include "TTLEventGenerator.h"

#include "TTLEventGeneratorEditor.h"


TTLEventGenerator::TTLEventGenerator()
    : GenericProcessor("TTL Event Generator")
{

}


TTLEventGenerator::~TTLEventGenerator()
{

}


AudioProcessorEditor* TTLEventGenerator::createEditor()
{
    editor = std::make_unique<TTLEventGeneratorEditor>(this);
    return editor.get();
}


void TTLEventGenerator::updateSettings()
{
    // create and add a TTL channel to the first data stream
    EventChannel::Settings settings{
            EventChannel::Type::TTL,
            "TTL Event Generator Output",
            "Default TTL event channel",
            "ttl.events",
            dataStreams[0]
    };

    ttlChannel = new EventChannel(settings);
    eventChannels.add(ttlChannel); // this pointer is now owned by the eventChannels array
    ttlChannel->addProcessor(processorInfo.get()); // make sure the channel knows about this processor

}

bool TTLEventGenerator::startAcquisition()
{
   counter = 0;
   state = false;

   return true;
}

void TTLEventGenerator::process(AudioBuffer<float>& buffer)
{
    // loop through the streams
    for (auto stream : getDataStreams())
    {
       // Only generate on/off event for the first data stream
       if(stream == getDataStreams()[0])
       {
          int totalSamples = getNumSamplesInBlock(stream->getStreamId());
          uint64 startSampleForBlock = getFirstSampleNumberForBlock(stream->getStreamId());

          int eventIntervalInSamples = (int) stream->getSampleRate();

          for (int i = 0; i < totalSamples; i++)
          {
             counter++;

             if (counter == eventIntervalInSamples)
             {

                state = !state;
                 
                int outputLine = 0;

                // add on or off event at the correct offset
                TTLEventPtr eventPtr = TTLEvent::createTTLEvent(ttlChannel,
                 startSampleForBlock + i,
                 outputLine,
                 state);

                addEvent(eventPtr, i);

                // reset counter
                counter = 0;

             }

             // extra check
             if (counter > eventIntervalInSamples)
                counter = 0;
          }
       }
    }
}


void TTLEventGenerator::handleTTLEvent(TTLEventPtr event)
{

}


void TTLEventGenerator::handleSpike(SpikePtr spike)
{

}


void TTLEventGenerator::handleBroadcastMessage(String message)
{

}


void TTLEventGenerator::saveCustomParametersToXml(XmlElement* parentElement)
{

}


void TTLEventGenerator::loadCustomParametersFromXml(XmlElement* parentElement)
{

}
