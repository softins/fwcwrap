<?php
	$fwcwrap = '/usr/local/bin/fwcwrap';
	$zone = 'remote';

	$myip = $_SERVER['REMOTE_ADDR'];

	if (@$_POST['submit'] == 'Add IP') {
		# strip out invalid characters
		$ret = exec("$fwcwrap -z $zone -a $myip", $output, $status);
		if ($status == 0) {
			header('Location: http://'.$_SERVER['SERVER_NAME'].$_SERVER['PHP_SELF']);
			exit;
		}
	} elseif (@$_POST['submit'] == 'Remove IP') {
		# strip out invalid characters
		$ret = exec("$fwcwrap -z $zone -d $myip", $output, $status);
		if ($status == 0) {
			header('Location: http://'.$_SERVER['SERVER_NAME'].$_SERVER['PHP_SELF']);
			exit;
		}
	}

	$ret = exec("$fwcwrap -z $zone -l $myip", $output, $status);
?>
<html>
<head>
<title>System IP address access</title>
</head>
<body>
<form method="post">
<table border=0 width="700px">
<tr><td>
<h1>Meet&Talk&trade; IP address access</h1></td></tr>
<tr><td><H2></h2></td></tr>
<?php if (@$status != 0) { ?>
<tr><td><h3 style="color: red">Your IP address <?= $myip ?> is NOT enabled for access</h3></td></tr>
<tr><td><h3>Click the button below to enable your IP address</h3></td></tr>
<tr><td align="center"><input type="submit" name="submit" value="Add IP" /></td></tr>
<?php } else { ?>
<tr><td><h3 style="color: green">Your IP address <?= $myip ?> is enabled for access</h3></td></tr>
<tr><td><h3>Click the button below to disable your IP address</h3></td></tr>
<tr><td align="center"><input type="submit" name="submit" value="Remove IP" /></td></tr>
<?php } ?>
</table>
</form>
</body>
</html>
