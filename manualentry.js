function doGet(e) {
  var ss = SpreadsheetApp.openByUrl("**"); //spreadsheet URL
  var sheet = ss.getSheetByName("Sheet2");

  addRecord(e,sheet);
}
 
function doPost(e) {
  var ss = SpreadsheetApp.openByUrl("**"); //spreadsheet URL
  var sheet = ss.getSheetByName("Sheet2");
 
  addRecord(e,sheet);
}

function addRecord(e,sheet) {
  var time = e.parameter.time ;
  var cumulativevolume = e.parameter.cumulativevolume ;
  var prevRow = sheet.getLastRow();
  var prevRange = sheet.getRange(prevRow, 2, 1, 1);
  sheet.appendRow([time,(Number(cumulativevolume) + Number(prevRange.getValues())),"Manual"]);
}
