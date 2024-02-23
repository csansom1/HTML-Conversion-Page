<?php

  session_start();

  $dir = __DIR__."/files/";

  if (isset($_POST["ok1"])) {
     if (isset($_POST["file"])) {
       $path = $dir.$_POST["file"];
       exec(__DIR__."/a4 $path 1", $output, $status);
     }
  }

  if (isset($_POST["ok2"])) {
    if ($_FILES) {
      $path = $dir.$_FILES["newFile"]["name"];
      move_uploaded_file($_FILES["newFile"]["tmp_name"], $path);
    }
  }

  if (isset($_POST["ok3"])) {
     if (isset($_POST["file"])) {
       $path = $dir.$_POST["file"];
       exec(__DIR__."/a4 $path", $output, $status);
       $_SESSION[$output[0]] = $output[1];
       exec("rm $path.html");
     }
  }

  if ($dh = opendir($dir)) {
    while (($filename = readdir($dh)) != false) {
      if ($filename != "" and $filename[0] != "." and strrpos($filename, '.info', -5) === false) {
        if (strrpos($filename, '.html', -5) === false) {
          $tf[] = $filename;
        } else {
          $hf[] = $filename;
        }
      }
    }
    exec(__DIR__."/db -show", $dbout, $dbstat);
    if ($dbout[0] == "csansom.files:") {
      foreach ($dbout as $dbfile) {
        if ($dbfile != "csansom.files:") {
          $hf[] = explode(" ", $dbfile)[2].".html";
        }
      }
    }
    if (sizeof($dbout) == 1) {
      session_unset();
      session_destroy();
    }
    sort($tf);
    sort($hf);
    closedir($dh);
  }



echo <<<_END1
  <html>
    <head>
      <style>
        .modal {
          display: none;
          position: fixed;
          z-index: 1;
          left: 0;
          top: 0;
          padding-top: 100px;
          width: 100%;
          height: 100%;
          background-color: rgba(0,0,0,0.4);
          }
        .modalForeground {
          background-color: #fefefe;
          margin: auto;
          padding: 20px;
          border: 1px solid #888;
          width: 80%;
        }
      </style>
    </head>
    <body>

      <button id="convert">Convert</button>
      <div id="cModal" class="modal">
        <div class="modalForeground">
          Please select a file to convert.<br>
          <button id="cClose">Close</button>
          <form method="post" action="a4.php" name="fileList">
            <input type="submit" name="ok1" value="OK"/><br>
_END1;

            foreach ($tf as $filename) {
              echo "<input type=\"radio\" name=\"file\" value=\"$filename\">$filename<br>";
            }

echo <<<_END2
          </form>
        </div>
      </div>

      <button id="upload">Upload</button>
      <div id="uModal" class="modal">
        <div class="modalForeground">
          Please select a file to upload.<br>
          <button id="uClose">Close</button>
          <form action="a4.php" method="post" enctype="multipart/form-data">
            <input type="submit" name="ok2" value="OK"/>
            <input type="file" name="newFile" size="8"/>
          </form>
        </div>
      </div>

      </form>
      <button id="dbStore">DB Store</button>
      <div id="dModal" class="modal">
        <div class="modalForeground">
          Please select a file to add to the database.<br>
          <button id="dClose">Close</button>
          <form method="post" action="a4.php" name="fileList">
            <input type="submit" name="ok3" value="OK"/><br>
_END2;
            foreach ($tf as $filename) {
              echo "<input type=\"radio\" name=\"file\" value=\"$filename\">$filename<br>";
            }

echo <<<_END3

          </form>
        </div>
      </div>

      <p>Text Files:
      <form>
_END3;

        foreach($tf as $file) {
          $tpath = substr($_SERVER["REQUEST_URI"], 0, -7);
          echo "<a href=\"https://".$_SERVER["HTTP_HOST"].$tpath."/files/$file\">$file</a><br>";
        }

echo <<<_END4

      <p>HTML Files:<p>
      <form>
_END4;

        foreach($hf as $file) {
          if (array_key_exists($file, $_SESSION)) {
            echo "<a href=\"javascript:getPage('$_SESSION[$file]')\">$file</a><br>";

          } else {
            $hpath = substr($_SERVER["REQUEST_URI"], 0, -7);
            echo "<a href=\"https://".$_SERVER["HTTP_HOST"].$hpath."/files/$file\">$file</a><br>";
          }
        }
        echo "</form>";

echo <<<_END5

      <script>
        var convertModal = document.getElementById("cModal");
        var uploadModal = document.getElementById("uModal");
        var dbModal = document.getElementById("dModal");

        var convertButton = document.getElementById("convert");
        var uploadButton = document.getElementById("upload");
        var dbButton = document.getElementById("dbStore");

        var convertClose = document.getElementById("cClose");
        var uploadClose = document.getElementById("uClose");
        var dbClose = document.getElementById("dClose");

        convertButton.onclick = function() {
          convertModal.style.display = "block";
        }

        uploadButton.onclick = function() {
          uploadModal.style.display = "block";
        }

        dbButton.onclick = function() {
          dbModal.style.display = "block";
        }

        convertClose.onclick = function() {
          convertModal.style.display = "none";
        }

        uploadClose.onclick = function() {
          uploadModal.style.display = "none";
        }

        dbClose.onclick = function() {
          dbModal.style.display = "none";
        }

        function getPage(file) {
          document.open();
          document.write(file);
          document.close();
        }

      </script>
    </body>
  </html>
_END5;
?>