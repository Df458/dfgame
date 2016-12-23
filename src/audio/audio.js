var actx;
var has_audio = true;

function init_audio()
{
    try {
        window.AudioContext = window.AudioContext||window.webkitAudioContext;
        actx = new AudioContext();
        has_audio = true;
    } catch(e) {
        alert("This browser doesn't support the Web AudioContextdio API.\nAudio will not play.");
    }
}

class AudioData
{
    constructor(path, name)
    {
        this.ready   = false;
        this.onready = function() { };
        this.buffer  = null;

        load_resource_to_buffer(path, name, function(request, user)
        {
            actx.decodeAudioData(request.response, function(buffer){ user.prepare(buffer) }, function() { alert("Failed to load audio!"); });
        }, this, "arraybuffer");
    }

    prepare(buffer)
    {
        this.ready = true;

        this.buffer = buffer;
        
        if(typeof this.onready === "function")
            this.onready();
    }
}

class AudioPlayer
{
    constructor()
    {
        this.ready   = false;
        this.loop    = false;
        this.playing = false;
        this.volume  = 1.0;

        if(arguments.length == 1 && typeof arguments[0] === "AudioData") {
            this.data = arguments[0];
            this.data.owner   = this;
            this.data.onready = function() { this.owner.prepare(); }
            if(this.data.ready)
                this.prepare();
        }
        else if(arguments.length >= 2) {
            this.data         = new AudioData(arguments[0], arguments[1]);
            this.data.owner   = this;
            this.data.onready = function() { this.owner.prepare(); }
        }
    }

    prepare()
    {
        this.ready = true;
        
        if(typeof this.onready === "function")
            this.onready();
    }

    play()
    {
        if(!this.ready)
            return;

        this.playing = true;

        this.source  = actx.createBufferSource();
        this.source.connect(actx.destination);
        this.source.buffer = this.data.buffer;
        this.source.connect(actx.destination);
        this.source.start(0);
    }

    pause()
    {
    }

    stop()
    {
        this.playing = false;
        this.source.stop();
    }
}
