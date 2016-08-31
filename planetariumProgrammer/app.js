
var localimages = [

];
rgbPattern = [0,1,2,3,0,1,2,3,0,1];
var FLICKR;

function loadLocalimages(eleId,n,images){
  var ele = document.getElementById(eleId);
  if(n==0){ele.innerHTML = null;}
  
  
  var img = document.createElement("img");
  img.className = "localimage";
  img.width = ele.clientWidth / 6;
  img.height = img.width;
  img.id = "localimage" + n;
  
  img.onload = function(){
    ele.appendChild(this);
    img.addEventListener("click",function(){
      console.log(this.id);
      console.log(this.src);
      FLICKR.addImagetoCanvas(this.src);
      FLICKR.addImage(this.src,"myimage");
    });
    if(n+1 < images.length){
      loadLocalimages(eleId,n+1,images);
    }else{
      console.log("load complete.");
    }
  }
  if(n < images.length){
    img.src = images[n];
  } 
}

window.addEventListener("resize",function(){
  var wrapper = document.querySelector("#wrapper");
  if(wrapper.clientWidth < wrapper.clientHeight){
    document.getElementById("wrapperleft").className = "margin0 width100 height60 left";
    document.getElementById("wrapperright").className = "margin0 width100 height40 left br";
  }else{
    document.getElementById("wrapperleft").className = "margin0 width50 height100 left";
    document.getElementById("wrapperright").className = "margin0 width50 height100 right";
  }
  var ele = document.getElementById("localimages");
  for(var i of document.querySelectorAll(".localimage")){
    i.width = ele.clientWidth / 6;
    i.height = i.width;
  }
});
window.addEventListener("load", function() {
  if(wrapper.clientWidth < wrapper.clientHeight){
    document.getElementById("wrapperleft").className = "margin0 width100 height60 left";
    document.getElementById("wrapperright").className = "margin0 width100 height40 left br";
  }else{
    document.getElementById("wrapperleft").className = "margin0 width50 height100 left";
    document.getElementById("wrapperright").className = "margin0 width50 height100 right";
  }
  


  console.log("Hello World!");
  

  var pattern;

  document.getElementById("readbtn").disabled = true;
  document.getElementById("readbtn").addEventListener("click",function(){
    outputColor(0,pattern);
  });
  document.getElementById("defaultbtn").addEventListener("click",function(){
    outputColor(0,rgbPattern);
  });

  document.getElementById("searchbtn").addEventListener("click",function(){
    var key = document.getElementById("keyword").value;
    console.log(key);
    FLICKR.load(key);
  });
  document.getElementById("keyword").addEventListener("keypress",function(event){
    if(event.keyCode == 13){
      var key = document.getElementById("keyword").value;
      console.log(key);
      FLICKR.load(key);
    }
  });


  $("#canvas").get(0).width = $("#canvas").width();
  $("#canvas").get(0).height = $("#canvas").height();


  FLICKR = new flickr("canvas");
  loadLocalimages("localimages",0,localimages);
  FLICKR.onload = function(){
    console.log("load");
    //colorRate = recognitionImgColor("canvas");
    recognitionImgColor("canvas",function(colorRate){
      decidePattern(colorRate,function(value){
        pattern = value;
        document.getElementById("readbtn").disabled = false;
      });
    });
    
    //pattern = decidePattern(colorRate);
    //document.getElementById("readbtn").disabled = false;
    //var colorRate = recognitionImgColor("myimage");
    //var pattern = decidePattern(colorRate);
    
    //outputColor(0,pattern);
  }
  //FLICKR.load("flower");
  //FLICKR.addImagetoCanvas("https://farm9.staticflickr.com/8841/27919514944_16847c135e.jpg");
});

function isFunc(func){
  if(typeof(func) === "function"){
    return true;
  }else{
    return false;
  }
}

function rgb2hsv(r,g,b){
  var h,s,v;
  var max = r;
  var min = r;
  var maxcolor = 0;
  if(max < g){
    max = g;
    maxcolor = 1;
  }
  if(max < b){
    max = b;
    maxcolor = 2;
  }
  if(min > g){
    min = g;
  }
  if(min > b){
    min = b;
  }

  if(maxcolor == 0){ 
    h = 60 * ( (g - b) / (max - min) );
  }else if(maxcolor == 1){
    h = 60 * ( (b - r) / (max - min) ) + 120;
  }else{
    h = 60 * ( (r - g) / (max - min) ) + 240;
  }
  if(h < 0){
    h += 360;
  }
  s = (max - min) / max;
  v = max;

  return {'h':h,'s':s,'v':v};
}

function detectColor(r,g,b){
  var hsv = rgb2hsv(r,g,b);
  var h = hsv.h;
  var s = hsv.s;
  var v = hsv.v;
  var y = 0.299 * (r/255) + 0.587 * (g/255) + 0.114 * (b/255);
  var tmpcolor;
  if(y > 0.8){
    //white
    tmpcolor = "WHITE";
  }else if(y < 0.1){
    //black
    tmpcolor = "BLACK";
  }else {
    if(h < 30 || 330 < h){
      //red
      tmpcolor = "RED";
    }else if(210 < h && h < 240){
      //blue
      tmpcolor = "BLUE";
    }else if(70 < h && h < 150){
      //green
      tmpcolor = "GREEN";
    }else if(40 < h && h < 70){
      //yellow
      tmpcolor = "YELLOW";
    }
  }
  return tmpcolor;
}

function recognitionImgColor(eleId,callback){
  var canvas = document.getElementById(eleId);
  var ctx = canvas.getContext("2d");

  var src = ctx.getImageData(0, 0, canvas.width, canvas.height);
  var dst = ctx.createImageData(canvas.width, canvas.height);

  var max=-1;
  var maxcolor;
  var colorRate = [0,0,0,0,0,0];
  for(var i=0; i<src.data.length;i+=4){
    /*
    if(max < src.data[i]){   max = src.data[i]; maxcolor = 0;}
    if(max < src.data[i+1]){ max = src.data[i]; maxcolor = 1;}
    if(max < src.data[i+2]){ max = src.data[i]; maxcolor = 2;}
    */
    var red = src.data[i];
    var green = src.data[i+1];
    var blue = src.data[i+2];
    var color = detectColor(red,green,blue);
    dst.data[i] = 0;
    dst.data[i+1] = 0;
    dst.data[i+2] = 0;
    dst.data[i+3] = 255;
    switch(color){
      case "WHITE":
        maxcolor = -1;
        dst.data[i] = 255;
        dst.data[i+1] = 255;
        dst.data[i+2] = 255;
        break;
      case "BLACK":
        maxcolor = -1;
        dst.data[i] = 0;
        dst.data[i+1] = 0;
        dst.data[i+2] = 0;
        break;
      case "RED":
        maxcolor = 0;
        dst.data[i] = 255;
        dst.data[i+1] = 0;
        dst.data[i+2] = 0;
        break;
      case "GREEN":
        maxcolor = 1;
        dst.data[i] = 0;
        dst.data[i+1] = 255;
        dst.data[i+2] = 0;
        break;
      case "BLUE":
        maxcolor = 2;
        dst.data[i] = 0;
        dst.data[i+1] = 0;
        dst.data[i+2] = 255;
        break;
      case "YELLOW":
        maxcolor = 3;
        dst.data[i] = 255;
        dst.data[i+1] = 255;
        dst.data[i+2] = 0;
        break;
    }
    if(0 <= maxcolor && maxcolor < 4){
      colorRate[maxcolor]++;
    }
  }
  console.log(colorRate);

  //ctx.putImageData(dst, 0, 0);
  if(isFunc(callback)){
    callback(colorRate);
  }

  //return colorRate;

}

function decidePattern(colorRate,callback){
  
  var sum = 0;
  colorRate.forEach(function(elm){
    sum += elm;
  });
  for(var i in colorRate){
    colorRate[i] /= sum;
    colorRate[i] = Math.round(colorRate[i]*10);
  }
  console.log(colorRate);
  sum = 0;
  colorRate.forEach(function(elm){
    sum += elm;
  });
  console.log(sum);
  var loop = sum;

  var p = new Array(10);
  for(var i=0;i<loop;i++){
    var rand = Math.floor(Math.random() * 4);
    sum = 0;
    colorRate.forEach(function(elm){
      sum += elm;
    });
    if(sum <= 0){break;}
    while(colorRate[rand]==0){
      rand = Math.floor(Math.random() * 4);
    }
    p[i]=rand;
    colorRate[rand]--;
  }
  console.log(p);
  if(isFunc(callback)){
    callback(p);
  }
  //return pattern;

}



function outputColor(n,pattern){
  console.log("output");
  var out = document.getElementById("programmer");
  switch(pattern[n]){
    case 0://red
      out.style.background = "#FF0000";
      break;
    case 1://green
      out.style.background = "#00FF00";
      break;
    case 2://blue
      out.style.background = "#0000FF";
      break;
    case 3://yellow
      out.style.background = "#FFFF00";
      break;
  }
  if(n+1 < pattern.length){
    setTimeout(function(){
      outputColor(n+1,pattern)
    },1000);
  }else{
    out.style.background = "#000000";
  }
}



