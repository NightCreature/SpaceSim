function OpenFile()
{
	var fileReader = new FileReader();
	var filesElement : HTMLInputElement = <HTMLInputElement>document.getElementById("myFile");
	var file : File = filesElement.files[0];
	fileReader.readAsText(file, "UTF-8");
	fileReader.onload = function (evt)
	{
		var fileString : string = evt.target.result as string;
		profilerDataReader.ParseJsonObject(fileString);
		profilerDataReader.DisplayEventDescriptors();
		ProcessFrameData();
	}
	fileReader.onerror = function (evt)
	{
		document.getElementById("error").innerHTML = "Error reading file";
	} 
};