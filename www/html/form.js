const form1 = document.getElementById("inputFile");

// Add 'submit' event handlers
form1.addEventListener("submit", (event) => {
    event.preventDefault();
	alert('The selected file will be uploaded');
    const data = new FormData(form1);
    fetch("/upload_file", {
        method: "post",
        body: data,
    })
    .then(response => {
        window.location.reload();
	})
});