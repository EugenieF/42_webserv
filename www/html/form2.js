// window.addEventListener("load", () => {
//     function onSuccess() {
//         document.location.href="./form_upload.html"
//     }
//     function sendData(formData) {
//         console.debug("Submitting form", {
//             name: formData.get("name"),
//             hamster: formData.get("hamster"),
//             color: formData.get("color"),
//         });

//         // Do something with the form data, e.g. send it to a server
// 		const request = new XMLHttpRequest();
// 		request.onreadystatechange = function() {
//             if (this.readyState == 4 && this.status == 200) {
//        			// Typical action to be performed when the document is ready:
//        			document.getElementById("post").innerHTML = request.responseText;
//     			}
// 			};
// 			request.open("POST", formData); // false for synchronous request, true for asynchronous 
//             request.send();

//         // Redirect to the next page on success (after successful call to server)
//         // onSuccess();
//     }

//     // Get the form elements
//     const form1 = document.getElementById("post");
//     const form2 = document.getElementById("without_cgi");

//     // Add 'submit' event handlers
//     form1.addEventListener("submit", (event) => {
//         event.preventDefault();
//         const data = new FormData(form1);
//         sendData(data);
//     });
//     form2.addEventListener("submit", (event) => {
//         event.preventDefault();
//         const data = new FormData(form2);
//         sendData(data);
//     });
// });