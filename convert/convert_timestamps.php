<?php
	$f = fopen( $argv[1],"r");
        $outfile = fopen( $argv[2], "w");
        while ( !feof($f) ) {
            $line = fgets($f);
            if ( preg_match('/\'(\d{4})-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\'/',$line)) {
		$line = str_replace("\\''","'",$line);
           	$line =  preg_replace('/\'(\d{4})-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\'/','$1',$line);
            }
            fputs($outfile, $line);
        }