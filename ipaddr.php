<?php
	$fwcwrap = '/usr/local/bin/fwcwrap';
	$zone = 'remote';
	$trusted = 'trusted';

	$myip = $_SERVER['REMOTE_ADDR'];

	if (@$_POST['submit'] == 'Add IP') {
		# strip out invalid characters
		$cmd = "$fwcwrap -z $zone -a $myip";
		$ret = exec($cmd, $output, $status);
		error_log("cmd=\"$cmd\", ret=$ret, output=" . join('|',$output) . ", status=$status");
		if ($status == 0) {
			header('Location: http://'.$_SERVER['SERVER_NAME'].$_SERVER['PHP_SELF']);
			exit;
		}
	} elseif (@$_POST['submit'] == 'Remove IP') {
		# strip out invalid characters
		$cmd = "$fwcwrap -z $zone -d $myip";
		$ret = exec($cmd, $output, $status);
		error_log("cmd=\"$cmd\", ret=$ret, output=" . join('|',$output) . ", status=$status");
		if ($status == 0) {
			header('Location: http://'.$_SERVER['SERVER_NAME'].$_SERVER['PHP_SELF']);
			exit;
		}
	}

	$cmd = "$fwcwrap -z $trusted -l $myip";
	$ret = exec($cmd, $output, $status);
	error_log("cmd=\"$cmd\", ret=$ret, output=" . join('|',$output) . ", status=$status");

	if ($status != 0) {
		$trusted = null;

		$cmd = "$fwcwrap -z $zone -l $myip";
		$ret = exec($cmd, $output, $status);
		error_log("cmd=\"$cmd\", ret=$ret, output=" . join('|',$output) . ", status=$status");
	}
?>
<!DOCTYPE html>
<html>
<head>
<title>IP address access</title>
<style type="text/css"><!--
body {
	font-family: Verdana, Arial, Helvetical, sans-serif;
	text-align: center;
}
h1 {
	text-align: center;
}
td.r {
	text-align: right;
}
div.wrap {
	display: inline-block;
	text-align: left;
	zoom: 1;
}
table.main {
	border: 0;
}
table.main td {
	padding-right: 2em;
	text-align: center;
}
--></style>
</head>
<body>
<div class="wrap">
<h1>IP address access</h1>
<form method="post">
<table class="main">
<tr><td><H2></h2></td></tr>
<?php if (@$status != 0) { ?>
<tr><td><h3 style="color: red">Your IP address <?= $myip ?> is NOT enabled for remote access</h3></td></tr>
<tr><td><h3>Click the button below to enable your IP address</h3></td></tr>
<tr><td align="center"><input type="submit" name="submit" value="Add IP" /></td></tr>
<?php } elseif ($trusted == null) { ?>
<tr><td><h3 style="color: green">Your IP address <?= $myip ?> is enabled for remote access</h3></td></tr>
<tr><td><h3>Click the button below to disable your IP address</h3></td></tr>
<tr><td align="center"><input type="submit" name="submit" value="Remove IP" /></td></tr>
<?php } else { ?>
<tr><td><h3 style="color: blue">Your IP address <?= $myip ?> is permanently trusted for remote access</h3></td></tr>
<?php } ?>
</table>
</form>
</div>
</body>
</html>
