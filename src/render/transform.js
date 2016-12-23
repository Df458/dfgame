class Transform2D {
    constructor()
    {
        this.mat = new Matrix();
        this.position = [0, 0];
        this.angle = 0;
        this.scaling = [1, 1];
    }

    translate(position, relative)
    {
        if(relative) {
            this.position[0] += position[0];
            this.position[1] += position[1];
        } else
            this.position = position;
        this.recalculate()
    }

    rotate(angle, relative)
    {
        if(relative)
            this.angle += angle;
        else
            this.angle = angle;
        this.recalculate()
    }

    scale(scale, relative)
    {
        if(relative) {
            this.scaling[0] *= scale[0];
            this.scaling[1] *= scale[1];
        } else
            this.scaling = scale;
        this.recalculate()
    }

    recalculate()
    {
        this.mat.ident();
        this.mat.rotate(this.angle, false);
        this.mat.translate(this.position[0], this.position[1], true);
        var s = new Matrix();
        s.scale(this.scaling[0], this.scaling[1], false);
        this.mat.mul(s);
    }
}
