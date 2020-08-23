PROTOCOL DOCUMENTATION

HTTP/1.1 RFC2616 (https://www.ietf.org/rfc/rfc2616.txt) was used in this program. A standard GET request is sent from the httpClient to the httpServer.

GET request take the following format:

`GET <path> <protocol>\r\n\r\n`

In httpClient case the request for index.html looks like the following:

`GET / HTTP/1.1\r\n\r\n`

\r\n\r\n is defined as the end of the header for HTTP/1.1 protocol.

Upon reciecpt of a request httpServer will issue a status code back to httpClient

`HTTP/1.1 200 OK`

or

`HTTP/1/1 404 NOT FOUND`
