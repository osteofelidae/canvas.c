// INTERNAL STRUCTS & FUNCTIONS ----------------------------------------------------------
typedef struct Request {  // Request
	char *method;  // GET, POST, etc.
	char *url;  // URL to send the request to
	struct curl_slist *headers;  // Headers
	char *content;  // Request content
}Request;
Request *malloc_req(  // Allocate memory for request struct
		char *method,  // GET, POST, etc.
		char *url,  // Url to send request to
		struct curl_slist *headers,  // Headers
		char *content  // Request content
	);
int free_req(  // Free Request memory
	Request *req  // Request to free
	);

typedef struct Response {  // Response to a request
	long code;  // Response status code
	char *content;  // Response content
}Response;
Response *malloc_Response(  // Allocate memory for Response struct
	long code,  // Status code
	char *content  // Response content
	);
int free_Response(  // Free Response struct memory
	Response *res  // Response struct to free
	);


// INTERNAL REQUEST FUNCTIONS ------------------------------------------------------------
int create_headers(  // Make header struct from raw strings
	struct curl_slist *headers,  // Headers list to add to
	char *headers_raw[], // Array of string headers
	int n_headers  // Number of headers in headers_raw
	);

size_t req_callback(  // Callback function for requests
	char *data,  // Data delivered by request
	size_t size,
	size_t nmemb,
	char *var  // String variable to write to
	);

int send_req(  // Send a request
	struct Request *req,  // Request data
	struct Response *res  // Struct to store response in
	);

