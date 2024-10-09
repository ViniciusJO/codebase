import pkg from "./package.json";
import net from "net";
import fs from "fs";

process.title = pkg.name + "_server";

const PORT = 8080;

net.createServer((socket: net.Socket) => {

  socket.on(`data`, (data) => {
    const request = data.toString();
    const url = request.split('\n')[0].split(' ')[1].slice(1).split('?')[0] || `index.html`;
    let fileContent = ""
    let found = true;

    try {
      fileContent = fs.readFileSync(url).toString();
    } catch(e) { console.error((e as Error).message); found = false; };

    const nfmsg = `Resource '${url}' not found\n`;
    const response = found 

      ?  `HTTP/1.1 200 OK\r\n` +
         `Content-Type: text/plain\r\n` +
         `Content-Length: ${fileContent.length}\r\n` +
         `Connection: close\r\n` +
         `\r\n` +
         fileContent 

      :  `HTTP/1.1 404 NotFound\r\n` +
         `Content-Length: ${nfmsg.length}\r\n` +
         `Connection: close\r\n` +
         `\r\n` +
         nfmsg;

    
    console.log(`\n\n-----------------REQUEST------------------\n${request}\n`);
    console.log(`-----------------RESPONSE-----------------\n${response}------------------------------------------\n\n\n`);
    socket.write(response);
    socket.end();
  });

}).listen(PORT, () => console.log(`Server listening on port ${PORT}...`));

