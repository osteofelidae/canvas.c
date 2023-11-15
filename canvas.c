// DEPENDENCIES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "canvas.h"
#include "internals.h"


// MACROS
#define N_CANVAS_HEADERS 3


// INTERNALS ============================================================================

// INTERNAL STRUCT FUNCTIONS ------------------------------------------------------------
Request *malloc_request(  // Allocate memory for Request struct
		char *method,  // GET, POST, etc.
		char *url,  // Url to send request to
		struct curl_slist *headers,  // Headers
		char *content  // Request content
	) {

	Request *req = malloc(sizeof(Request));  // Allocate Request struct memory

	req->method = method;  // Set Request members
	req->url = url;
	req->headers = headers;
	req->content = content;

	return req;

}
int free_request(  // Free Request struct memory
	Request *req  // Request struct to free
	) {

	free(req);  // Free Request struct

	return 0;

}

Response *malloc_response(  // Allocate memory for Response struct
	long code,  // Status code
	char *content  // Response content
	) {

	Response *res = malloc(sizeof(Response));  // Allocate Response struct memory

	res->content = content;  // Set Response members

	return res;

}
int free_response(  // Free Response struct memory
	Response *res  // Response struct to free
	) {

	free(res);  // Free Response struct

	return 0;

}


// INTERNAL REQUEST FUNCTIONS -----------------------------------------------------------
int create_headers(  // Make header struct from raw strings
	struct curl_slist *headers,  // Headers list to add to
	char *headers_raw[], // Array of string headers
	int n_headers  // Number of headers in headers_raw
	) {

	for (int index = 0; index < n_headers; index++) {  // Iterate over raw headers
		headers = curl_slist_append(  // Add current raw header to headers list
			headers, 
			headers_raw[index]
			);
	}

	return 0;

}

size_t req_callback(  // Callback function for requests
	char *data,  // Data delivered by request
	size_t size,
	size_t nmemb,
	char *var  // String variable to write to
	) {

	if (sizeof(data) <= sizeof(var)) {  // If output buffer is large enough
		strcpy(  // Copy result to output var
			var,
			data
			);
	} else {
		fprintf(  // Print error to stderr
			stderr,
			"Request callback: output buffer too small"
			);
	}

	return size * nmemb;  // Return same size so that CURL knows callback was successful

}

int send_req(  // Send a request
	struct Request *req,  // Request data
	struct Response *res  // Struct to store response in
	) {

	CURL *curl = curl_easy_init();  // CURL handle
	CURLcode curl_result;  // CURL result

	curl_easy_setopt(  // Specify request type
		curl,
		CURLOPT_CUSTOMREQUEST,
		req->method
		);

	curl_easy_setopt(  // Set CURL url
		curl,
		CURLOPT_URL, 
		req->url
		);

	if (req->headers != NULL) {  // If headers are provided
		curl_easy_setopt(  // Set CURL headers
			curl,
			CURLOPT_HTTPHEADER,
			req->headers
			);
	}

	if (req->content != NULL) {  // If content is provided
		curl_easy_setopt(  // Set content
			curl,
			CURLOPT_POSTFIELDS,
			req->content
			);
	}

	curl_easy_setopt(  // Set callback function
		curl,
		CURLOPT_WRITEFUNCTION,
		req_callback
		);

	curl_easy_setopt(  // Set callback function parameter
		curl,
		CURLOPT_WRITEDATA,
		res->content
		);

	curl_result = curl_easy_perform(curl);  // Do request

	if (curl_result == CURLE_OK) {  // If request succeed

		curl_easy_getinfo(  // Response code
			curl,
			CURLINFO_RESPONSE_CODE,
			&res->code
			);

	} else {  // If request failed

		fprintf(  // Print error to stderr
			stderr,
			"%s request: request to %s failed (%s)\n",
			req->method,
			req->url,
			curl_easy_strerror(curl_result)
			);

		return -1;

	}

	curl_easy_cleanup(curl);  // Clean up

	return 0;

}


// INTERNAL CANVAS FUNCTIONS ------------------------------------------------------------
int create_canvas_headers(  // Create headers for Canvas API requests
		struct curl_slist *headers,  // Headers struct to append to
		char *token,  // OAUTH token
		char *type  // Request type
	) {

	char auth_header[92] = "Authorization: Bearer ";  // Base
	strcat(auth_header, token);  // Concat base and OAUTH token

	char *format;

	if (  // POST and PUT requests are HTML form encoded
		strcmp(type, "POST") == 0 ||
		strcmp(type, "PUT") == 0
		) {

		format = "application/x-www-form-urlencoded";

	} else {  // Other requests are JSON encoded

		format = "application/json";

	}

	char format_header[128] = "content-type: ";  // Base
	strcat(format_header, format);  // Concat base and format

	printf(format_header);

	char *headers_arr[N_CANVAS_HEADERS] = {  // Raw string headers
		auth_header,
		format_header,
		"Accept: application/json+canvas-string-ids"  // Force all returned IDs to be strings
	};

	create_headers(  // Create headers
		headers,
		headers_arr,
		N_CANVAS_HEADERS
		);

	return 0;

}


// PUBLICS ==============================================================================

// PUBLIC STRUCT FUNCTIONS --------------------------------------------------------------



// PUBLIC CANVAS FUNCTIONS --------------------------------------------------------------
int init() {  // Initialize environment

	curl_global_init(CURL_GLOBAL_DEFAULT);  // Global CURL init

	return 0;

}

int clean() {  // Clean environment

	curl_global_cleanup();  // Global CURL cleanup

	return 0;

}