#!/bin/bash/

export	REQUEST_METHOD="GET" \
		REDIRECT_STATUS="200" \
		QUERY_STRING="name=&hamster=&color=#997a8d" \
		PATH_TRANSLATED="" \
		PATH_INFO="" \
		SCRIPT_NAME="/cgi/cgi_form.php" \
		CONTENT_LENGTH="0" \
		CONTENT_TYPE="application/x-httpd-php"
export	ROOT="/www/html"
export	REQUEST_URI="/cgi/cgi_form.php"
export	SCRIPT_FILENAME=$PWD$ROOT$SCRIPT_NAME

echo $SCRIPT_FILENAME
php-cgi ./www/html/cgi/cgi_form.php
