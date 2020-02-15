function init_events()
{
    document.getElementById("fileUpID").addEventListener("change",eventoninput,false);
}

function eventoninput(e) {
    console.log(e.target.files.length);
    document.getElementById("FileDropList").innerHTML = "<li onclick='document.getElementById("+"\"fileUpID\""+").click()'>Hier klicken um andere Dateien auszuwaehlen</li>";

    for(let i = 0; i < e.target.files.length; i++)
    {
	let list = document.createElement("li");

	let p = document.createElement("p");
	p.innerHTML = "<div>File selected: "+e.target.files[i].name+"</div>";

	let prog = document.createElement("progress");
	let span = document.createElement("span");
	prog.style["float"] = "right";
	prog.style["display"] = "inline-block";
	prog["max"] = "100";

	span.innerHTML = "0%";
	span.style["float"] = "right";
	span.style["margin-right"] = "2rem";

	p.appendChild(prog);
	p.appendChild(span);
	list.appendChild(p);
	list.onclick = function(){this.parentNode.removeChild(this);};
	document.getElementById("FileDropList").appendChild(list);
	list.progentry = prog;
	list.spanentry = span;
	list.file = e.target.files[i];
    }

}

function ShowStatus(x,color)
{
    let ban = document.getElementById("status_banner");
    ban.innerHTML = x;
    ban.style.color = color;
    window.scrollTo(0,0);
}

function Upload(x,key)
{
    let xhr = new XMLHttpRequest();
    let create_ocr = document.getElementById("languageoptenabled").checked;
    let language_ocr = document.getElementById("languageopt").value;
    if(x.force_overwrite == true)
	xhr.open("POST","/upload?scanid="+key+"&create_ocr="+create_ocr+"&language="+language_ocr+"&filename="+x.file.name+"&forced=true",true);
    else
	xhr.open("POST","/upload?scanid="+key+"&create_ocr="+create_ocr+"&language="+language_ocr+"&filename="+x.file.name,true);

    x.progentry.value = 0;
    xhr.onload = function() {
	if(xhr.status == 403)
	{
	    if(xhr.responseText == "file_exists" && x.force_overwrite!=true) //File exists go for overwrite
	    {
		x.style.background = "yellow";
		x.children[0].children[0].innerHTML = "File already exists: "+x.file.name+", overwrite?";
		let butt = document.createElement("compare");
		butt.innerHTML = "Compare Files";
		butt.style.background = "blue";
		butt.style.float = "right";
		butt.style.padding = "0.5rem";
		butt.style.cursor = "pointer";
		butt.onclick = function() {
		    OpenModal(x.file,key);
		}
		x.children[0].children[0].appendChild(butt);
		x.children[0].children[1].style.display = "none";
		x.children[0].children[2].style.display = "none";
		document.getElementById("nobut").onclick = function() {
		    HideModal();
		    x.innerHTML = "<p><div>Canceled upload of: "+x.file.name+" Keeping existing file</div></p>";
		    x.style.background = "red";
		    x.upload_finished = true;
		    UploadAll();
		}

		document.getElementById("yesbut").onclick = function() {
		    HideModal();
		    x.force_overwrite = true;
		    x.style.background = "";
		    x.children[0].children[1].style.display = "";
		    x.children[0].children[2].style.display = "";
            x.innerHTML = "<p><div>Overwriting existing file with: " +x.file.name+"</div></p>";
		    butt.parentNode.removeChild(butt);
		    Upload(x,key);
		}
		return;

		let yes = document.createElement("button");
		let no = document.createElement("button");
		yes.style.background = "red";
		yes.innerHTML = "Yes";
		no.style.background = "green";
		no.innerHTML = "No";
		x.children[0].children[0].appendChild(yes);
		x.children[0].children[0].appendChild(no);
		x.children[0].children[1].style.display = "none";
		x.children[0].children[2].style.display = "none";

		yes.onclick = function() {
		 		}

		no.onclick = function() {
		}
	    }
	    else
	    {
		return ShowStatus("Server returned error: "+xhr.responseText,"red");
	    }
	    //File exists already ask for override permission
	}
	else if(xhr.status==200)
	{
	    x.style.background = "green";
	    x.upload_finished = true;
	    x.progentry.value = 100;
	    x.spanentry.innerHTML = 100+"%";
	    UploadAll();	//Recursivly upload rest of the files
	}
	else
	    return ShowStatus("Unknown error while uploading please dont try again and show this message to the server administrators","red");
    }

    xhr.onprogress = function (event) {
	if(event.lengthComputable)
	{
	    let xval = Math.round(event.loaded/event.total*100);
	    x.progentry.value = xval;
	    x.spanentry.innerHTML = xval+"%";
	}
    };

    xhr.send(x.file);
}

function ExtractKey()
{
    let permalink = document.getElementById("Permalink").value;
    if(permalink=="")
	return "";

    let arr = permalink.split("/");
    let finalKey = "";
    for(var i = 0; i < arr.length; i++)
    {
	if(arr[i]=="itemKey")
	{
	    finalKey = arr[i+1];
	    break;
	}
    }
    if(finalKey=="" && (arr.length==1))
	finalKey = permalink;
    return finalKey;
}

function HideModal()
{
    document.getElementById("modal").style.display = "none";
}

function OpenModal(fl,key)
{
    let ending = fl.name.substr(fl.name.lastIndexOf(".")+1);

    if(ending == 'jpg'||ending=='JPG'||ending=='png'||ending=='PNG')
	ShowImageSlide(fl,key);
    else
	ShowOCRSlide(fl,key);
}

function ShowOCRSlide(fl,key)
{
    document.getElementById("modal").style.display = "block";
    let oldf = document.getElementById("oldfile");
    let newf = document.getElementById("newfile");
    let oldfileparagraph = document.createElement("p");
    let newfileparagraph = document.createElement("p");
    oldfileparagraph.style.background = "white";
    newfileparagraph.style.background = "white";
    oldf.appendChild(oldfileparagraph);
    newf.appendChild(newfileparagraph);

    let xhr = new XMLHttpRequest();
    xhr.open("GET","/books/"+key+"/"+fl.name,true);
    xhr.onload = function() {
	if(xhr.status!=200)
	{
	    oldfileparagraph.innerHTML = "Could not load file sorry for that :(";
	}
	else
	{
	    oldfileparagraph.innerHTML = xhr.responseText;
	}
    }
    xhr.send();

    if (FileReader) {
	var fr = new FileReader();
	fr.onload = function () {
	    newfileparagraph.innerHTML = fr.result;
	}
	fr.readAsText(fl);
    }
    else
    {
	newfileparagraph.innerHTML = "Could not load file sorry for that :(";
    }
}

function ShowImageSlide(fl,key)
{
    document.getElementById("modal").style.display = "block";
    let kk = document.getElementById("oldfile");
    kk.innerHTML = "";
    document.getElementById("newfile").innerHTML = "";

    let val = document.createElement("img");
    val.src = "/books/"+key+"/"+fl.name;
    val.style["max-height"] = "100%";
    val.style["max-width"] = "100%";
    val.style.margin = "auto";
    val.style.display = "inherit";
    kk.appendChild(val);

    if (FileReader) {
	var fr = new FileReader();
	fr.onload = function () {
	    let valnew = document.createElement("img");
	    valnew.src = fr.result;
	    valnew.style["max-height"] = "100%";
	    valnew.style["max-width"] = "100%";
	    valnew.style.margin = "auto";
	    valnew.style.display = "inherit";
	    document.getElementById("newfile").appendChild(valnew);
	}
	fr.readAsDataURL(fl);
    }
}

function ShowBookContentLink()
{
    let lnk = ExtractKey();
    document.getElementById("BookContentLink").innerHTML = "<a href='/books/"+lnk+"/' target='_blank'>Book File List Index</a><a style='margin-left: 4rem;' href='/ShowMetadata.html?scanId="+lnk+"' target='_blank'> Book Metadata</a>";
}

function UploadAll()
{
    let elem = document.getElementById("fileUpID");
    elem.onclick = function(){return false;};

    let finalKey = ExtractKey();
    if(finalKey=="")
	return ShowStatus("Either you forgot to type in the item key or the permalink is in the wrong format!","red");



    let val = document.getElementById("FileDropList").children;
    for(let i = 1; i < val.length; i++)
    {
	if(val[i].upload_finished==undefined || val[i].upload_finished == null)
	{
	    val[i].onclick = function() { };
	    Upload(val[i],finalKey);
	    return ShowStatus("There are files that already exist, you may select whether to overwrite or not for each file", "red");
	}
    }
    ShowStatus("Uploaded all files successfully! In order to find results in the new book you have to restart the server! (The Server automatically restarts over night)","green");
}

function show_language(x)
{
    if(x.checked)
    {
	document.getElementById("languageopt").style.display = "inline-block";
    }
    else
    {
	document.getElementById("languageopt").style.display = "none";
    }
}


window.addEventListener("load",function(){initialise("uploadbooklink");init_events();},false);
