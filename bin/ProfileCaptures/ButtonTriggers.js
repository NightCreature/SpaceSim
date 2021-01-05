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
		console.println("Error in data");
		console.println(evt.target.result);
		document.getElementById("error").innerHTML = "Error reading file";
	} 
};