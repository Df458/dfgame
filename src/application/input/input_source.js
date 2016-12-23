var input_list;

class Input
{
    constructor()
    {
        input_list.push(this);
        this.keys = [];
        this.init();
    }

    init() {}

    bindKey(key)
    {
        this.keys.push(key);
    }

    unmapKey(key)
    {
        this.keys.remove(key);
    }

    ondown(e)
    {
        this.activate(true, e.keyCode);
    }

    onup(e)
    {
        this.activate(false, e.keyCode);
    }

    activate(input, key)
    {
    }

    update()
    {
    }
}

class ActionInput extends Input
{
    activate(input, key)
    {
        if(input && this.keys.indexOf(key) > -1)
            if(this.onactive && typeof this.onactive === "function")
                this.onactive();
    }
}

class AxisInput extends Input
{
    init()
    {
        this.digital = [0, 0];
        this.analog  = [0, 0];
        this.bindings = [];
        this.active_list = [];
    }

    bindKey(key)
    {
        if(arguments.length < 3) {
            // TODO: Warning here
            return;
        }
        super.bindKey(key);

        this.bindings.push([arguments[1], arguments[2]]);
        this.active_list.push(false);
    }

    unmapKey(key)
    {
        var index = this.keys.indexOf(key);
        if(input && index > -1) {
            this.active_list.splice(index, 1);
            this.bindings.splice(index, 1);
            super.unmapKey(key);
        }
    }

    activate(input, key)
    {
        var index = this.keys.indexOf(key);
        if(index > -1) {
            this.active_list[index] = input;
        }
    }

    update()
    {
        this.digital = [0, 0];
        for(var i = 0; i < this.keys.length; i++) {
            if(this.active_list[i]) {
                var index = this.bindings[i][0] ? 0 : 1;
                var value = this.bindings[i][1] ? 1 : -1;
                this.digital[index] += value;
            }
        }
        if(this.digital[0] != 0)
            this.digital[0] /= Math.abs(this.digital[0]);
        if(this.digital[1] != 0)
            this.digital[1] /= Math.abs(this.digital[1]);

        if(Math.abs(this.analog[0] - this.digital[0]) >= 0.1) {
            this.analog[0] += (this.digital[0] - this.analog[0]) / 15;
        } else {
            this.analog[0] = this.digital[0];
        }
        if(Math.abs(this.analog[1] - this.digital[1]) >= 0.1) {
            this.analog[1] += (this.digital[1] - this.analog[1]) / 15;
        } else {
            this.analog[1] = this.digital[1];
        }
    }
}

function init_input()
{
    input_list = [];

    document.addEventListener("keydown", function(event) { input_list.forEach(function(input) { input.ondown(event); }); });
    document.addEventListener("keyup", function(event) { input_list.forEach(function(input) { input.onup(event); }); });
}

function update_input()
{
    input_list.forEach(
        function(input) {
            input.update();
        }
    );
}
