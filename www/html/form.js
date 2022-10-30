function uploadData() {
    // alert('The selected file will be uploaded');
    console.log("Debug");
    const input = document.getElementById("file");
    const formData = new FormData();
    formData.append(file.name, input.files[0]);
    console.log(formData);
    // const response = fetch(`/form_upload`, {
    //     method: 'POST',
    //     credentials: 'include',
    //     body: formData,
    // })
    // .then(response => {
    //     // window.location.href = "form_upload.html";
    //     console.log("OK");
    // })
}