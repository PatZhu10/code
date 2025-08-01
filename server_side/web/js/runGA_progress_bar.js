function runGA_progress_bar() {
    var progress = document.querySelector('.progress');
    var progress_bar = document.querySelector('.progress-bar');
    var percent = document.querySelector('.percent');
    var count = 1;
    var per = 2;
    var loading = setInterval(animate, 690);

    percent.style.display = "block";
    progress.style.display = "block";
    progress_bar.style.display = "block";

    function animate() {
        if(count == 100 && per == 200) {
            clearInterval(loading);
        }
        else
        {
            count += 1;
            per += 2;
            progress.style.width = per + 'px';
            percent.textContent = count + '%';
        }
    }
}