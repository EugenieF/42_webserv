function uploadData() {
    // alert('The selected file will be uploaded');
    const input = document.getElementById("file");
    const formData = new FormData();
    formData.append(file.name, input.files[0]);
    const response = fetch(`/form_upload`, {
        method: 'POST',
        credentials: 'include',
        body: formData,
    })
    .then(response => {
        // window.location.href = "form_upload_ok.html";
    })
    .catch(response => {
        console.log("Error");  
    })
}

// function uploadData() {
//     // alert('The selected file will be uploaded');
//     const input = document.getElementById("file");
//     const formData = new FormData();
//     formData.append(file.name, input.files[0]);
//     const response = fetch(file.name, {
//         method: 'POST',
//         credentials: 'include',
//         body: input.files[0],
//     })
//     .then(response => {
//         // window.location.href = "form_upload_ok.html";
//     })
//     .catch(response => {
//         console.log("Error");  
//     })
// }