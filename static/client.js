let ws = new WebSocket("ws://localhost:9999", "lws-minimal-server-echo");

ws.onopen = function(e) {
    console.log("[open] Connection established");
    ws.binaryType = "arraybuffer"
    ws.send('bump')
};

ws.onmessage = function(event) {
    if (event.data instanceof Blob || event.data instanceof ArrayBuffer) {
        let dec = new TextDecoder()
        console.log(dec.decode(event.data))
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
