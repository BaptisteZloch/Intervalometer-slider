$(document).ready(function () {
    $("#myRange4").change(function () {
        var val = $("#myRange4").val();
        $("#valeurangle").text(val);
    });
});

$(document).ready(function () {
    $("#myRange3").change(function () {
        var val = $("#myRange3").val();
        $("#valeurlongueur").text(val);
    });
});


document.addEventListener('DOMContentLoaded', function () {
    var checkbox = document.getElementById("cblong");
    checkbox.addEventListener('change', function () {
        if (checkbox.checked) {
            document.getElementById("myRange3").style.visibility = "visible";
        } else {
            document.getElementById("myRange3").style.visibility = "hidden";
        }
    });
    var checkbox2 = document.getElementById("cbangle");
    checkbox2.addEventListener('change', function () {
        if (checkbox2.checked) {
            document.getElementById("myRange4").style.visibility = "visible";
        } else {
            document.getElementById("myRange4").style.visibility = "hidden";
        }
    });
});