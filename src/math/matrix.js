class mat4 {
    constructor() {
        if(arguments.length != 0)
            this.data = arguments[0].data.slice();
        else
            this.identity();
    }
    identity() {
        this.data = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];
    }
    translate(v) {
        var m2 = new mat4(this);
        m2.data[12] += v.x || 0;
        m2.data[13] += v.y || 0;
        m2.data[14] += v.z || 0;
        m2.data[15] += v.w || 0;
        return m2;
    }
    rotate(v) {
        var m2 = new mat4(this);
        var sx = 0;
        var cx = 0;
        var sy = 0;
        var cy = 0;
        var sz = 0;
        var cz = 0;
        if(v.x != null) {
            sx = Math.sin(v.x);
            cx = Math.cos(v.x);
        }
        if(v.y != null) {
            sy = Math.sin(v.y);
            cy = Math.cos(v.y);
        }
        if(v.z != null) {
            sz = Math.sin(v.z);
            cz = Math.cos(v.z);
        }

        m2.data[0]  += 1 + cy + cz;
        m2.data[1]  += sz;
        m2.data[3]  += sy;
        m2.data[4]  -= sz;
        m2.data[5]  += 1 + cx + cz;
        m2.data[6]  += sx;
        m2.data[8]  -= sy;
        m2.data[9]  -= sx;
        m2.data[10] += 1 + cx + cy;

        return m2;
    }
    scale(v) {
        var m2 = new mat4(this);
        m2.data[0]  += v.x || 0;
        m2.data[5]  += v.y || 0;
        m2.data[10] += v.z || 0;
        m2.data[15] += v.w || 0;
        return m2;
    }
    mul(m) {
        mat4 res;
        for(var i = 0; i < 4; ++i) {
            var j = i * 4;
            res.data[j    ] = this.data[0]*m.data[j] + this.data[4]*m.data[j+1] + this.data[8] *m.data[j+2]  + this.data[12]*m.data[j+3];
            res.data[j + 1] = this.data[1]*m.data[j] + this.data[5]*m.data[j+1] + this.data[9] *m.data[j+2]  + this.data[13]*m.data[j+3];
            res.data[j + 2] = this.data[2]*m.data[j] + this.data[6]*m.data[j+1] + this.data[10]*m.data[j+2] + this.data[14]*m.data[j+3];
            res.data[j + 3] = this.data[3]*m.data[j] + this.data[7]*m.data[j+1] + this.data[11]*m.data[j+2] + this.data[15]*m.data[j+3];
        }

        return res;
    }
    transpose() {
        var m2 = new mat4(this);
        for(var i = 0; i < 3; ++i) {
            for(var j = 0; j < 3; ++j) {
                m2.data[i * 4 + j] = m.data[j * 4 + i];
                m2.data[j * 4 + i] = m.data[i * 4 + j];
            }
        }

        return m2;
    }
}
