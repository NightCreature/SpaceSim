function OpenFile()
{
	var fileReader = new FileReader();
	var file = document.getElementById("myFile").files[0];
	fileReader.readAsText(file, "UTF-8");
	fileReader.onload = function (evt)
	{
		profilerDataReader.ParseJsonObject(evt.target.result);
		profilerDataReader.DisplayEventDescriptors();
		ProcessFrameData();
	}
	fileReader.onerror = function (evt)
	{
		document.getElementById("demo").innerHTML = "Error reading file";
	} 
};