function handleErrors(response) {
    if (!response.ok) {
        throw Error(response.statusText);
    }
    return response;
}

function uploadData() {
    // alert('The selected file will be uploaded');
    const input = document.getElementById("file");
    const formData = new FormData();
    formData.append(file.name, input.files[0]);
    const response = fetch(`/form_upload`, {
        method: 'POST',
        credentials: 'include', // send cookies, even in CORS
        body: formData,
    })
    .then( response => {
        if (response.ok) {
            window.location.href = "form_upload_ok.html";
        } else {
            // console.log(response.text());
            throw Error(response.text());
        }
    })
    .catch( error => {
        console.log(error);
        // console.log(response.status, response.statusText);
    })
}



//     .then(handleErrors)
//     .then(function(response) {
//         console.log("ok");
//     }).catch(function(error) {
//         console.log(error);
//     });
// }



//     .then(response => {
//         if (response.ok) {
//             console.log("Ok");  
//             // window.location.href = "form_upload_ok.html";
//         }
//         else {
//             console.log("Error");
//         }
//     })
//     .catch(response => {
//         console.log("Error catch");
//     })
// }
