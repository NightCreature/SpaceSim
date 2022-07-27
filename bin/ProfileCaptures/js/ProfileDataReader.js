class ProfilerDataReader {
    ParseJsonObject(text) {
        this.m_profilerData = JSON.parse(text);
        if (this.m_profilerData == null) {
            var newContent = document.createTextNode("Failed to parse JSON data");
            var errorDiv = document.getElementById("error");
            errorDiv.appendChild(newContent);
        }
    }
    DisplayEventDescriptors() {
        var table = document.createElement('table');
        table.id = "EventDescriptors";
        var row = document.createElement('tr');
        var names = Object.keys(this.m_profilerData.EventDescriptors[0]);
        for (var counter = 0; counter < names.length; ++counter) {
            var cell = document.createElement('th');
            cell.textContent = names[counter];
            row.appendChild(cell);
        }
        table.appendChild(row);
        for (var count = 0; count < this.m_profilerData.EventDescriptors.length; ++count) {
            row = document.createElement('tr');
            var EventDescription = this.m_profilerData.EventDescriptors[count];
            var names = Object.keys(EventDescription);
            for (var count2 = 0; count2 < names.length; ++count2) {
                cell = document.createElement('td');
                var paragraph = document.createElement('p');
                paragraph.appendChild(document.createTextNode(EventDescription[names[count2]]));
                cell.appendChild(paragraph);
                row.appendChild(cell);
            }
            table.appendChild(row);
        }
        var infoDiv = document.getElementById("EventDescriptors");
        infoDiv.appendChild(table);
    }
    GetProfilerData() { return this.m_profilerData; }
}
//# sourceMappingURL=ProfileDataReader.js.map