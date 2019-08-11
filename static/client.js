let ws = new WebSocket("ws://localhost:9999", "lws-minimal-server-echo");

ws.onopen = function(e) {
    console.log("[open] Connection established");
    ws.binaryType = "arraybuffer"
};

ws.onmessage = function(event) {
    if (event.data instanceof Blob || event.data instanceof ArrayBuffer) {
        let dec = new TextDecoder()
        document.forms.cli.output.value += dec.decode(event.data)
    } else {
        console.log(event.data)
    }
};

ws.onclose = function(event) {
    if (event.wasClean) {
        console.log(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
    } else {
        console.log('[close] Connection died');
    }
};

ws.onerror = function(error) {
    console.log(`[error] ${error.message}`);
};

document.forms.cli.onsubmit = function(event) {
    document.forms.cli.output.value += '\n'
    ws.send(document.forms.cli.input.value + '\n')
    document.forms.cli.input.value = ''
    event.preventDefault();
    return false;
}
