var resource_path = "";

function construct_extended_resource_path(path, name)
{
    if(resource_path) {
        if(path)
            return resource_path + "/" + path + "/" + name;
        return resource_path + "/" + name;
    }
    if(path)
        return path + "/" + name;
    return name;
}

function load_resource_to_buffer(path, name, callback, user)
{
    var request = new XMLHttpRequest();
    request.addEventListener("load", function() { callback(request, user); });
    request.open("GET", construct_extended_resource_path(path, name));
    if(arguments.length >= 5) {
        request.responseType = arguments[4];
    }
    request.send();
}
