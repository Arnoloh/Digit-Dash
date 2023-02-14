<?php 
	
	
		try{
			$db = new PDO('mysql:host=chronodjess.mysql.db;dbname=chronodjess', 'chronodjess', 'qc39Jessy');
			$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		} catch(PDOException $e){
			echo $e;
		
		}