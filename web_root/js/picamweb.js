function tableCreate() {
  var body = document.getElementsByTagName('body')[0];
  var tbl = document.createElement('table');
  tbl.style.width = '100%';
  tbl.setAttribute('border', '1');
  var tbdy = document.createElement('tbody');
  for (var i = 0; i < 3; i++) {
    var tr = document.createElement('tr');
    for (var j = 0; j < 2; j++) {
      if (i == 2 && j == 1) {
        break
      } else {
        var td = document.createElement('td');
        td.appendChild(document.createTextNode('\u0020'))
        i == 1 && j == 1 ? td.setAttribute('rowSpan', '2') : null;
        tr.appendChild(td)
      }
    }
    tbdy.appendChild(tr);
  }
  tbl.appendChild(tbdy);
  body.appendChild(tbl)
}


function createSetings () {
  var settings = document.getElementById("usrCtrlSettings");
  console.log(settings);
  // var content = document.createTextNode("<YOUR_CONTENT>");

  for (var j = 0; j < 3; j++) {
  	// create 3 row
	  var new_row = document.createElement('div');
	  new_row.className = "row";
	  settings.appendChild(new_row);
	  // create 4 div
  	for (var i =0; i <4; i++) {
// <div class="col-sm-3">
//   <div class="option">
//      <label for="volume">Volume</label>
//      <input type="range" min="0" max="1" 
// 				step="0.1" name="volume" id="volume" value="1">
//   </div>
// </div>
			 var new_div = document.createElement('div');
			 new_div.className = "col-sm-3 option";

			  //  Make label
    		var label = document.createElement('label');
        label.setAttribute('for','volume');
        label.textContent = "Volume";
        new_div.appendChild(label);

        //  Make Input
    		var input = document.createElement('input');
        input.setAttribute('type','range');
        input.setAttribute('min','0');
        input.setAttribute('max','10');
        input.setAttribute('step','1');
        input.setAttribute('name','volume');
        input.setAttribute('id','volume');
        input.setAttribute('value','1');
        new_div.appendChild(input);

			 new_row.appendChild(new_div);
  	}

  	settings.appendChild(new_row);
  }
  
}


// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

// Helper function to display upload status
var setStatus = function(text) {
  document.getElementById('fwUpFileStt').innerText = text;
};

// When user clicks on a button, trigger file selection dialog
var button = document.getElementById('fwUpFileBt');
button.onclick = function(ev) {
  input.click();
};

// Send a large blob of data chunk by chunk
var sendFileData = function(name, data, chunkSize) {
  var sendChunk = function(offset) {
    var chunk = data.subarray(offset, offset + chunkSize) || '';
    var opts = {method: 'POST', body: chunk};
    var url = '/upload?offset=' + offset + '&name=' + encodeURIComponent(name);
    setStatus(
        'sending bytes ' + offset + '..' + (offset + chunk.length) + ' of ' +
        data.length);
    fetch(url, opts).then(function(res) {
      if (chunk.length > 0) sendChunk(offset + chunk.length);
    });
  };
  sendChunk(0);
};

// If user selected a file, read it into memory and trigger sendFileData()
var input = document.getElementById('fwUpFile');
input.onchange = function(ev) {
  if (!ev.target.files[0]) return;
  var f = ev.target.files[0], r = new FileReader();
  r.readAsArrayBuffer(f);
  r.onload = function() {
    ev.target.value = '';
    sendFileData(f.name, new Uint8Array(r.result), 4096);
  };
};
