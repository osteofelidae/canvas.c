// DEPENDENCIES -----------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "canvas.h"
#include "internals.h"


// INTERNALS ======================================================================================

// FUNCTIONS --------------------------------------------------------------------------------------
int lprintf(  // Verbose print
	char *text,  // Text to be printed
	int type,  // 1 for error, 2 for normal
	int verbose  // Pass in verbose param from enclosing function
	) {

	if (type <= verbose) {printf("%s\n", text);}  // Print if allowed

	return 0;

}

size_t callback(  // Callback function for requests
	char *data,  // Response data
	size_t size,
	size_t nmemb,
	char *var  // String variable to write to
	) {

	strcpy(  // Copy data over
		var, 
		data
		);

	return size * nmemb;  // Return correct int for CURL

}

int doreq(  // Send a request
	char var[],  // Var to write to
	char *token,  // OAUTH token
	char *method,  // Request method (GET, POST, etc.)
	char *url,  // URL to send request to
	char *content,  // POST content
	int verbose  // 0: none; 1: errors only; 2: all
	) {

	lprintf(  // Verbose print
		"doreq: creating request",
		2,
		verbose
		);

	CURL *curl;  // CURL handle
	CURLcode curlcode;  // CURL result
	long rescode;  // Response status code

	curl = curl_easy_init();  // Initialize
	
	char auth[128] = "Authorization: Bearer ";  // [TODO optimize size] Auth header
	strcat(auth, token);  // Append token to auth header

	char format[128] = "content-type:application/";  // [TODO optimize size] Content-type header
	if (  // POST and PUT requests are HTML form encoded
		strcmp(method, "POST") == 0 || 
		strcmp(method, "PUT") == 0
		) {
		strcat(format, "x-www-form-urlencoded");
	} else {  // Other requests are json encoded
		strcat(format, "json");
	}

	struct curl_slist *headers = NULL;  // Headers slist

	headers = curl_slist_append(headers, auth);  // Add generated headers to slist
	headers = curl_slist_append(headers, format);

	curl_easy_setopt(  // Set request method
		curl,
		CURLOPT_CUSTOMREQUEST,
		method
		);
	
	curl_easy_setopt(  // Set request headers
		curl,
		CURLOPT_HTTPHEADER,
		headers
		);
	
	curl_easy_setopt(  // Set CURL url
		curl,
		CURLOPT_URL, 
		url
		);
	
	if (content != NULL) {  // If content is provided, set request content
		curl_easy_setopt(
			curl,
			CURLOPT_POSTFIELDS,
			content
			);
	}
	
	curl_easy_setopt(  // Set callback function
		curl,
		CURLOPT_WRITEFUNCTION,
		callback
		);

	curl_easy_setopt(  // Set callback param var
		curl,
		CURLOPT_WRITEDATA,
		var
		);

	lprintf(  // Verbose print
		"doreq: sending request",
		2,
		verbose
		);
	
	curlcode = curl_easy_perform(curl);  // Do request

	if (curlcode != CURLE_OK && verbose == 1) {  // If request failed

		lprintf(  // Verbose print
			"doreq: request failed: invalid parameters",
			1,
			verbose
		);

		return 1;  // Return with error code

	} else {

		curl_easy_getinfo(  // Get response code
			curl,
			CURLINFO_RESPONSE_CODE,
			&rescode
			);

		if (2 <= verbose) {printf("doreq: response received: code %ld\n", rescode);}

		switch (rescode) {
			case 401:
				lprintf(  // Verbose print
					"doreq: authentication error: invalid OAUTH token",
					1,
					verbose
				);
				return 1;  // Return with error code
		}	// TODO MORE CASES

	}

	curl_easy_cleanup(curl);  // Free CURL handle

	curl_slist_free_all(headers);  // Free headers slist

	return 0;

}


// PUBLICS ========================================================================================

// FUNCTIONS --------------------------------------------------------------------------------------
int init() {

	curl_global_init(CURL_GLOBAL_DEFAULT);
	return 0;

}

int clean() {

	curl_global_cleanup();
	return 0;

}

