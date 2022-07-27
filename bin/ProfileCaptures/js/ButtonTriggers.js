function OpenFile() {
    var fileReader = new FileReader();
    var filesElement = document.getElementById("myFile");
    var file = filesElement.files[0];
    fileReader.readAsText(file, "UTF-8");
    fileReader.onload = function (evt) {
        var fileString = evt.target.result;
        profilerDataReader.ParseJsonObject(fileString);
        profilerDataReader.DisplayEventDescriptors();
        ProcessFrameData();
    };
    fileReader.onerror = function (evt) {
        document.getElementById("error").innerHTML = "Error reading file";
    };
}
;
//# sourceMappingURL=ButtonTriggers.js.map