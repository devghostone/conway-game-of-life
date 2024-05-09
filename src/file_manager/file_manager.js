let reader= new FileReader();

function offerFileAsDownload(filename, mime) {
    mime = mime || "application/octet-stream";
  
    let content = Module.FS.readFile(filename);
    console.log(`Offering download of "${filename}", with ${content.length} bytes...`);
  
    var a = document.createElement('a');
    a.download = filename;
    a.href = URL.createObjectURL(new Blob([content], {type: mime}));
    a.style.display = 'none';
  
    document.body.appendChild(a);
    a.click();
    setTimeout(() => {
      document.body.removeChild(a);
      URL.revokeObjectURL(a.href);
    }, 2000);
  }

function triggerFileInput(){
  document.getElementById('fileInput').click();
}

function loadFile(){
    let files = document.getElementById('fileInput').files;
    let file=files[0];
    reader.addEventListener('loadend', printFile);
    reader.readAsArrayBuffer(file);
}

function printFile(e){
    let result=reader.result;
    const uint8_view = new Uint8Array(result);

    Module.FS.writeFile('user_input.gocgol', uint8_view)
    Module._fileLoadCompleted();
}