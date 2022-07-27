var profilerDataReader = new ProfilerDataReader();

function findEventDescriptorForHash(hash, eventDescriptors)
{
    for (let counter = 0; counter < eventDescriptors.length; ++counter)
    {
        let desc = eventDescriptors[counter];
        if (desc["Event Hash"] == hash)
        {
            return desc;
        }
    }

    return null;
}

function ProcessFrameData()
{
    let profilerData = profilerDataReader.GetProfilerData();

    let eventDescriptors = profilerData.EventDescriptors;
    let frames = profilerData.Frames;

    let frame = frames[0];

    let table = document.createElement('table');
    table.id = "FrameData";
    let row = document.createElement('tr');
    let cell = document.createElement('th');
    cell.textContent = "Frame Number";
    row.appendChild(cell);
    cell = document.createElement('th');
    cell.textContent = "Event Data";
    row.appendChild(cell);
    table.appendChild(row);

    for (let frameCounter = 0; frameCounter < frames.length; ++frameCounter)
    {
        let frame = frames[frameCounter];
        let frameRow = document.createElement('tr');
        let frameNumberCell = document.createElement('td');
        frameNumberCell.textContent = frameCounter.toString();
        frameRow.appendChild(frameNumberCell);
        let frameEventDataCell = document.createElement('td');

        let eventTable = document.createElement('table');
        eventTable.id = "EventData";
        let row = document.createElement('tr');
        let cell = document.createElement('th');
        cell.textContent = "Event Name";
        row.appendChild(cell);
        cell = document.createElement('th');
        cell.textContent = "Duration";
        row.appendChild(cell);
        eventTable.appendChild(row);

        for (let counter = 0; counter < frame.Events.length; ++counter)
        {
            let event = frame.Events[counter];
            //Lookup event hash in the descriptors
            let descriptor = findEventDescriptorForHash(event["Event Hash"], eventDescriptors);
            if (descriptor != null)
            {
                let row = document.createElement('tr');
                let cell = document.createElement('td');
                cell.textContent = descriptor["Event Name"];
                row.appendChild(cell);
                cell = document.createElement('td');
                let numberOfTicks = event["End Timestamp"] - event["Start Timestamp"];
                let date = new Timespan(numberOfTicks, profilerData.TimerResolution );
                //cell.textContent = (event["End Timestamp"] - event["Start Timestamp"]) * 1000 / profilerData.TimerResolution + " ms  ";
                cell.textContent += date.GetTimeString() + " (" + numberOfTicks + " ticks)";
                row.appendChild(cell);
                eventTable.appendChild(row);
            }
        }

        
        frameEventDataCell.appendChild(eventTable);
        frameRow.appendChild(frameEventDataCell);
        table.appendChild(frameRow);
    }
    
    let paragraph = document.createElement('p');
    paragraph.textContent = "Trace contains " + frames.length + " Frames ";
    paragraph.textContent += "CPU Frequency is: " + profilerData.TimerResolution + " QueryPerformance Counter Ticks per second (Not actual ticks / instructions)";
    
    let infoDiv = document.getElementById('info');
    infoDiv.appendChild(paragraph);
    infoDiv.appendChild(document.createElement('br'));
    infoDiv.appendChild(table);
}