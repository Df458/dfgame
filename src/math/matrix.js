// TODO: Use proper decomposition
class Matrix
{
    constructor()
    {
        if(arguments.length != 0)
            this.data = arguments[0].data.slice();
        else
            this.ident();
    }

    ident()
    {
        this.data = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1];
    }

    translate(x, y, relative)
    {
        if(relative) {
            this.data[12] += x;
            this.data[13] += y;
        } else {
            this.data[12] = x;
            this.data[13] = y;
        }
    }

    // TODO: Relative support
    rotate(angle, relative)
    {
        var s = Math.sin(angle);
        var c = Math.cos(angle);

        this.data[0] =  c;
        this.data[1] =  s;
        this.data[4] = -s;
        this.data[5] =  c;
    }

    // TODO: Make relative vs. absolute
    scale(x, y, relative)
    {
        this.data[0] *= x;
        this.data[5] *= y;
    }

    ortho(left, right, down, up, near, far)
    {
        this.ident();
        this.data[0]  = 2 / (right - left);
        this.data[12] = (right + left) / (right - left) * -1;
        this.data[5]  = 2 / (up - down);
        this.data[13] = (up + down) / (up - down) * -1;
        this.data[10] = 2 / (far - near);
        this.data[14] = (far + near) / (far - near) * -1;
        this.data[15] = 1;
    }

    mul(m)
    {
        for(var i = 0; i < 4; ++i) {
            var j = i * 4;
            this.data[j    ] = this.data[0]*m.data[j] + this.data[4]*m.data[j+1] + this.data[8]*m.data[j+2]  + this.data[12]*m.data[j+3];
            this.data[j + 1] = this.data[1]*m.data[j] + this.data[5]*m.data[j+1] + this.data[9]*m.data[j+2]  + this.data[13]*m.data[j+3];
            this.data[j + 2] = this.data[2]*m.data[j] + this.data[6]*m.data[j+1] + this.data[10]*m.data[j+2] + this.data[14]*m.data[j+3];
            this.data[j + 3] = this.data[3]*m.data[j] + this.data[7]*m.data[j+1] + this.data[11]*m.data[j+2] + this.data[15]*m.data[j+3];
        }
    }
}
