let ws = new WebSocket("ws://localhost:9999", "lws-minimal-server-echo")

ws.onopen = function(e) {
    console.log("[open] Connection established")
    ws.binaryType = "arraybuffer"
}

function create_node_from_template(template_id, container_id, new_node_id) {
    var template = document.getElementById(template_id)
    var container = document.getElementById(container_id)
    var clone = document.importNode(template.content, true)
    container.appendChild(clone)
    container.lastElementChild.id = new_node_id
    return document.getElementById(new_node_id)
}

metadata = {}
ws.onmessage = function(event) {
    if (event.data instanceof Blob || event.data instanceof ArrayBuffer) {
        var cli = document.getElementById('cli-' + metadata.pid)
        if (cli == null) {
            cli = create_node_from_template('cli-template', 'cli-container', 'cli-' + metadata.pid)
            cli.pid.value = metadata.pid
            cli.onsubmit = function(event) {
                var cli = event.target
                input = cli.input.value + '\n'
                cli.input.value = ''
                cli.output.value += input
                ws.send(input)
                event.preventDefault()
                return false
            }
        }
        let dec = new TextDecoder()
        new_output = dec.decode(event.data)
        if (new_output.startsWith("(gdb) ")) {
            cli.style.display = "block";
        }
        cli.output.value += new_output
        cli.output.scrollIntoView(false)
    } else {
        metadata = JSON.parse(event.data)
    }
}

ws.onclose = function(event) {
    if (event.wasClean) {
        console.log(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`)
    } else {
        console.log('[close] Connection died')
    }
}

ws.onerror = function(error) {
    console.log(`[error] ${error.message}`)
}
