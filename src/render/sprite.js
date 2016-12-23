class Animation
{
    constructor()
    {
        this.orients    = 1;
        this.dimensions = [0, 0];
        this.origin     = [0, 0];
        this.length     = 1;
        this.delay      = 1;
        this.autoplay   = true;
        this.autoloop   = false;
        this.speed_mod  = 1;

        this.atlas_box = [0, 0, 0, 0];
        this.handle = "idle";
    }

    getTexCoords(frame)
    {
        var c = [0, 0, 0, 0];
        c[0] = this.atlas_box[0];
        c[1] = this.atlas_box[1];
        c[2] = this.atlas_box[2];
        c[3] = this.atlas_box[3];
        c[0] += frame * this.dimensions[0];
        c[2] /= this.length;
        if(this.orients != 0)
            c[3] /= this.orients;

        c[0] /= this.owner.tex.width;
        c[1] /= this.owner.tex.height;
        c[2] /= this.owner.tex.width;
        c[3] /= this.owner.tex.height;

        return c;
    }
}

class Spriteset
{
    constructor()
    {
        this.animations = [];
        this.loaded     = 0;
        this.ready      = false;

        if(arguments.length >= 2) {
            this.path = arguments[0];
            this.name = arguments[1];
            load_resource_to_buffer(arguments[0], arguments[1], function(request, user) { user.prepare(request.response) }, this, "document");
        }
    }

    prepare(doc)
    {
        var root = null;
        for(var i = 0; i < doc.children.length; ++i) {
            if(doc.children[i].localName == "spriteset") {
                root = doc.children[i];
                break;
            }
        }
        if(!root)
            return;

        for(var i = 0; i < root.children.length; ++i) {
            if(root.children[i].localName == "animation") {
                var index = this.animations.length;
                this.animations.push(new Animation);
                this.animations[index].owner = this;

                if(root.children[i].hasAttribute("name")) {
                    this.animations[index].handle = root.children[i].getAttribute("name");
                }
                if(root.children[i].hasAttribute("length")) {
                    this.animations[index].length = root.children[i].getAttribute("length");
                }
                if(root.children[i].hasAttribute("speed")) {
                    this.animations[index].delay = root.children[i].getAttribute("speed");
                }
                if(root.children[i].hasAttribute("orients")) {
                    this.animations[index].orients = root.children[i].getAttribute("orients");
                }
                if(root.children[i].hasAttribute("loop")) {
                    this.animations[index].autoloop = root.children[i].getAttribute("loop") === "true";
                }
                if(root.children[i].hasAttribute("play")) {
                    this.animations[index].autoplay = root.children[i].getAttribute("play") === "true";
                }
                if(root.children[i].hasAttribute("origin_x")) {
                    this.animations[index].origin[0] = root.children[i].getAttribute("origin_x");
                }
                if(root.children[i].hasAttribute("origin_y")) {
                    this.animations[index].origin[1] = root.children[i].getAttribute("origin_y");
                }
                if(root.children[i].hasAttribute("file")) {
                    // TODO: Make this a normal image to cut down on WebGL
                    // texture creation
                    this.animations[index].tex         = new Texture(this.path, root.children[i].getAttribute("file"));
                    this.animations[index].tex.owner   = this;
                    this.animations[index].tex.onready = function() { this.owner.prepareAnimation(index); }
                }
            }
        }

        if(this.animations.length == 0)
            return;
    }

    prepareAnimation(index)
    {
        this.loaded++;
        if(this.loaded != this.animations.length)
            return;

        var available = [[0, 0, 1024, 1024]];
        var anim_boxes = [];
        for(var i = 0; i < this.animations.length; ++i) {
            var selected = -1;
            for(var j = 0; j < available.length; ++j) {
                if((available[j][2] >= this.animations[i].tex.width && available[j][3] >= this.animations[i].tex.height) && (selected < 0 || available[j][2] * available[j][3] > available[selected][2] * available[selected][3])) {
                    selected = j;
                }
            }
            if(selected == -1) {
                // TODO: Resize the atlas
            }
            anim_boxes.push(available[selected]);
            available.splice(selected, 1);
            if(anim_boxes[i][2] > this.animations[i].tex.width) {
                available.push([anim_boxes[i][0] + this.animations[i].tex.width, anim_boxes[i][1], anim_boxes[i][2] - this.animations[i].tex.width, anim_boxes[i][3]]);
                anim_boxes[i][2] = this.animations[i].tex.width;
            }
            if(anim_boxes[i][3] > this.animations[i].tex.height) {
                available.push([anim_boxes[i][0], anim_boxes[i][1] + this.animations[i].tex.height, anim_boxes[i][2], anim_boxes[i][3] - this.animations[i].tex.height]);
                anim_boxes[i][3] = this.animations[i].tex.height;
            }
            this.animations[i].atlas_box = anim_boxes[i];
        }
        this.tex        = new Texture();
        this.tex.width  = 1024;
        this.tex.height = 1024;
        this.tex.tex    = ctx.createTexture();
        ctx.bindTexture(ctx.TEXTURE_2D, this.tex.tex);

        ctx.texImage2D(ctx.TEXTURE_2D, 0, ctx.RGBA, 1024, 1024, 0, ctx.RGBA, ctx.UNSIGNED_BYTE, null);
        for(var i = 0; i < this.loaded; ++i) {
            ctx.texSubImage2D(ctx.TEXTURE_2D, 0, anim_boxes[i][0], anim_boxes[i][1], ctx.RGBA, ctx.UNSIGNED_BYTE, this.animations[i].tex.img);
        }

        ctx.texParameterf(ctx.TEXTURE_2D,  ctx.TEXTURE_MIN_FILTER,   ctx.NEAREST);
        ctx.texParameterf(ctx.TEXTURE_2D,  ctx.TEXTURE_MAG_FILTER,   ctx.NEAREST);
        // ctx.texParameteri(ctx.TEXTURE_2D,  ctx.TEXTURE_WRAP_S,       ctx.CLAMP_TO_BORDER);
        // ctx.texParameteri(ctx.TEXTURE_2D,  ctx.TEXTURE_WRAP_T,       ctx.CLAMP_TO_BORDER);
        // ctx.texParameterfv(ctx.TEXTURE_2D, ctx.TEXTURE_BORDER_COLOR, [ 0, 0, 0, 0 ]);

        this.tex.ready = true;
        this.ready = true;

        for(var i = 0; i < this.animations.length; ++i) {
            this.animations[i].dimensions = [this.animations[i].tex.width / this.animations[i].length, this.animations[i].tex.height];
        }

        if(typeof this.onready === "function")
            this.onready();
    }
}

class Sprite
{
    constructor()
    {
        if(arguments.length == 1) {
            this.set = arguments[0];
        } else if(arguments.length == 2) {
            this.set = new Spriteset(arguments[0], arguments[1]);
        } else {
            return;
        }

        this.index    = 0;
        this.position = 0;
        this.playing  = false;
    }

    update()
    {
        if(!this.playing || !this.set.ready)
            return false;

        this.position += 0.15; // TODO: Make this not hardcoded
        if(this.position >= this.set.animations[this.index].length) {
            if(this.set.animations[this.index].autoloop) {
                while(this.position >= this.set.animations[this.index].length) {
                    this.position -= this.set.animations[this.index].length
                }
                return true;
            } else {
                this.position = this.set.animations[this.index].length - 1;
                this.playing = false;
            }
            return false;
        }

        return true;
    }

    setAnimation(handle)
    {
        this.position = 0;
        this.index = 0;
        for(var i = 0; i < this.set.animations.length; ++i) {
            if(this.set.animations[i].handle === handle) {
                this.index = i;
                this.playing = this.set.animations[i].autoplay; 
            }
        }
    }

    draw(camera, transform, color, scale)
    {
        if(!this.set.loaded)
            return;
        var a = this.set.animations[this.index];
        render_quad(camera, transform, color, this.set.tex, this.set.animations[this.index].getTexCoords(Math.floor(this.position)), scale);
    }

    play()
    {
        this.playing = true;
    }

    pause()
    {
        this.playing = false;
    }

    stop()
    {
        this.position = 0;
        this.playing  = false;
    }
}
