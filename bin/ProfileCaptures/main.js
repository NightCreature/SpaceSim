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
    table.id = "EventData";
    let row = document.createElement('tr');
    let cell = document.createElement('th');
    cell.textContent = "Event Name";
    row.appendChild(cell);
    cell = document.createElement('th');
    cell.textContent = "Duration";
    row.appendChild(cell);
    table.appendChild(row);
    let headerCell = document.createElement('th');

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
            cell.textContent = event["End Timestamp"] - event["Start Timestamp"];
            row.appendChild(cell);
            table.appendChild(row);
        }
    }

    document.getElementById('info').appendChild(table);
}