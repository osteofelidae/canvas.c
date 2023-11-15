// INTERNAL STRUCTS & FUNCTIONS ---------------------------------------------------------
typedef struct Request {  // Request
	char *method;  // GET, POST, etc.
	char *url;  // URL to send the request to
	struct curl_slist *headers;  // Headers
	char *content;  // Request content
}Request;
Request *malloc_request(  // Allocate memory for request struct
		char *method,  // GET, POST, etc.
		char *url,  // Url to send request to
		struct curl_slist *headers,  // Headers
		char *content  // Request content
	);
int free_request(  // Free Request memory
	Request *req  // Request to free
	);

typedef struct Response {  // Response to a request
	long code;  // Response status code
	char *content;  // Response content
}Response;
Response *malloc_response(  // Allocate memory for Response struct
	long code,  // Status code
	char *content  // Response content
	);
int free_response(  // Free Response struct memory
	Response *res  // Response struct to free
	);


// INTERNAL REQUEST FUNCTIONS -----------------------------------------------------------
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

// INTERNAL CANVAS FUNCTIONS ------------------------------------------------------------
int create_canvas_headers(  // Create headers for Canvas API requests
		struct curl_slist *headers,  // Headers struct to append to
		char *token,  // OAUTH token
		char *method  // Request type
	);

int send_canvas_req(
	Response *res,  // Response struct to write to
	char *method,  // Request method
	char *token,  // OAUTH token
	char *url,  // URL to request to
	char *content  // Content of the request, if applicable
	);