var id;

function main_loop_begin(f)
{
    id = setInterval(main_loop_step, 12, f);
}

function main_loop_step(f)
{
    f();
}

function main_loop_request_end()
{
    if(id)
        clearInterval(id);
}
