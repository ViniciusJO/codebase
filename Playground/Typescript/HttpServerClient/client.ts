import http from 'http';

const options: http.RequestOptions = {
  hostname: 'localhost',
  port: 8080,
  path: process.argv.length > 2 ? `/`+process.argv[process.argv.length-1] : '/',
  method: 'GET',
};

const req: http.ClientRequest = http.request(options, (res: http.IncomingMessage) => {
  let data: string = '';
  let dataOnArray: string[] = [];

  res.on('data', (chunk: string) => {
    data += chunk;
    dataOnArray.push(chunk.toString());
  });

  res.on('end', () => {
    console.log(`Response ${res.statusCode}:\n\n${data}\n`);
    //console.log(dataOnArray,`\n`)
  });
});

req.on('error', (e: Error) => {
  console.error(`Problem with request: ${e.message}`);
});

req.end();

