
var flickr = function(eleId){
  this.domain = "https://api.flickr.com/services/rest/";
  this.method = "flickr.photos.search";
  this.api_key = "Your flickr api key";
  this.eleId = eleId;
  this.onload;
  this.images = [];
  this.numLoadimage = 6;
}

flickr.prototype = {
  send:function(url,callback){
    console.log("send "+url);
    //var xhr = new XMLHttpRequest({mozSystem: true});
    var xhr = new XMLHttpRequest();
    //xhr.open('GET', url, true);
    xhr.open('GET', url, true);
    xhr.onreadystatechange = function(){
      if (xhr.readyState === 4 && xhr.status === 200){
        //console.log(xhr.responseText);
        if(typeof(callback)==="function"){
          callback(xhr.responseText);
        }
      }
    };
    xhr.send(null);
  },
  load:function(query){
    this.getImageList(query);
  },
  getImageList:function(query){
    var self = this;
    var url = self.domain + "?method=" + self.method + "&api_key=" + self.api_key + "&text=" + query;

    self.send(url,function(res){
      console.log(res);
      //var xmlDoc = $.parseXML( res );
      //$xml = $(xmlDoc);
      if(self.numLoadimage < $(res).find("photo").length){
        for(var i =0;i<self.numLoadimage;i++){
          self.images[i] = self.getImageUrl($(res).find("photo")[i]);
        }
        var loadUrl = self.images[0];
        self.addImagetoCanvas(loadUrl);
        self.addImage(loadUrl,"myimage");
        loadLocalimages("localimages",0,self.images);
      }
      
    });
  },
  getImageUrl:function(photo){
    var self = this;
    if(!photo){
      return;
    }
    console.log(photo);

    var id = $(photo).attr("id");
    var secret = $(photo).attr("secret");
    var server = $(photo).attr("server");
    var farm = $(photo).attr("farm");

    var url = "https://farm" + farm + ".staticflickr.com/" +server+ "/" + id +"_" + secret + ".jpg";
    console.log(url);
    return url;
  },
  addImage:function(src,eleId){
    var ele
    if(eleId){
      ele = document.getElementById(eleId);
    }else{
      ele = document.getElementById(self.eleId);
    }
    ele.innerHTML = null;
    var img = document.createElement("img");
    img.className = "loadimage";
    img.src = src;

    //img.width = 100;
    //img.height = 100;
    img.onload = function(){
      ele.appendChild(this);
    }
  },
  addImagetoCanvas:function(src){
    var self = this;
    var canvas = document.getElementById(self.eleId);
    if ( ! canvas || ! canvas.getContext ) { return false; }
    var cw = canvas.width;
    var ch = canvas.height;
    var ctx = canvas.getContext('2d');
    ctx.fillStyle = 'rgb(0, 0, 0)';
    ctx.beginPath();
    ctx.fillRect(0,0,cw,ch);
    var img = new Image();
    img.crossOrigin = "anonymous";
    
    img.onload = function(){
      console.log("img w:"+img.width);
      console.log("img h:"+img.height);
      if(img.width < img.height){
        scale = (ch / img.height);
      }else{
        scale = (cw / img.width);
      }
      dw = img.width * scale;
      dh = img.height * scale;
      console.log(cw,ch,dw,dh,scale);
      ctx.drawImage(img, 0, 0,img.width,img.height,0,0,dw,dh);
      if(typeof(self.onload)==="function"){
        self.onload();
      }
    
    }
    img.src = src;
  }
}

