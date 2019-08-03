let ws = new WebSocket("ws://localhost:9999", "lws-minimal-server-echo");

ws.onopen = function(e) {
    console.log("[open] Connection established");
    ws.send('bump')
};

ws.onmessage = function(event) {
    console.log(`[message] Data received from server: ${event.data}`);
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
