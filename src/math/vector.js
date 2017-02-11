class vec2 {
    constructor() {
        this.x = (this.arguments.length > 0) ? this.arguments[0] : 0;
        this.y = (this.arguments.length > 1) ? this.arguments[1] : 0;
    }
    len_squared() {
        return Math.pow(this.x, 2) + Math.pow(this.y, 2);
    }
    len() {
        return Math.sqrt(this.len_squared());
    }
    normalize() {
        var l = this.len()
        if(l == 0) {
            throw new UserException('Attempting to normalize a vec2 of 0 length is undefined.')
        }

        x /= l;
        y /= l;
    }
    add(vec) {
        return new vec2(x + vec.x, y + vec.y);
    }
    sub(vec) {
        return new vec2(x - vec.x, y - vec.y);
    }
    dot(vec) {
        return (x * vec.x) + (y * vec.y);
    }
}
class vec3 {
    constructor() {
        this.x = (this.arguments.length > 0) ? this.arguments[0] : 0;
        this.y = (this.arguments.length > 1) ? this.arguments[1] : 0;
        this.z = (this.arguments.length > 2) ? this.arguments[2] : 0;
    }
    len_squared() {
        return Math.pow(this.x, 2) + Math.pow(this.y, 2) + Math.pow(this.z, 2);
    }
    len() {
        return Math.sqrt(this.len_squared());
    }
    normalize() {
        var l = this.len()
        if(l == 0) {
            throw new UserException('Attempting to normalize a vec3 of 0 length is undefined.')
        }

        x /= l;
        y /= l;
        z /= l;
    }
    add(vec) {
        return new vec3(x + vec.x, y + vec.y, z + vec.z);
    }
    sub(vec) {
        return new vec3(x - vec.x, y - vec.y, z - vec.z);
    }
    dot(vec) {
        return (x * vec.x) + (y * vec.y) + (z * vec.z);
    }
    cross(vec) {
        return vec3((y * vec.z) - (z * vec.y), (z * vec.x) - (x * vec.z), (x * vec.y) - (y * vec.x))
    }
}
class vec4 {
    constructor() {
        this.x = (this.arguments.length > 0) ? this.arguments[0] : 0;
        this.y = (this.arguments.length > 1) ? this.arguments[1] : 0;
        this.z = (this.arguments.length > 2) ? this.arguments[2] : 0;
        this.w = (this.arguments.length > 3) ? this.arguments[3] : 0;
        if
    }
    len_squared() {
        return Math.pow(this.x, 2) + Math.pow(this.y, 2) + Math.pow(this.z, 2) + Math.pow(this.w, 2);
    }
    len() {
        return Math.sqrt(this.len_squared());
    }
    normalize() {
        var l = this.len()
        if(l == 0) {
            throw new UserException('Attempting to normalize a vec4 of 0 length is undefined.')
        }

        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }
    add(vec) {
        return new vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
    }
    sub(vec) {
        return new vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
    }
    dot(vec) {
        return (x * vec.x) + (y * vec.y) + (z * vec.z) + (w * vec.w);
    }
}
