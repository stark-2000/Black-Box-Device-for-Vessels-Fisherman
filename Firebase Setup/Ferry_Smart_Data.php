<?php
require 'firebaseLib.php';
$deviceId = $_GET["deviceid"];
$Vtype = $_GET["vesseltype"];
$country = $_GET["country"];
$IMOnumber = $_GET["IMONumber"];
$MMSInumber = $_GET["MMSINumber"];
$Size = $_GET["Size"];
$GT = $_GET["GT"];
$Vesselname = $_GET["VesselName"];
$Ownername = $_GET["OwnerName"];
$Speed = $_GET["speed"];
$Course = $_GET["course"];
$Status = $_GET["Status"];
$Distancefromcoast = $_GET["Distancefromcoast"];
$Tamperproof = $_GET["Tamperproof"];
$emerg = $_GET["emergency"];
$backup = $_GET["backup"];
$latitude = $_GET["lat"];
$longitude = $_GET["lon"];


const DEFAULT_URL = 'https://ferry-smart.firebaseio.com';
const DEFAULT_TOKEN = 'EAwWIIIOJOnRgmuTxO5AIuwMAMPyRk7DUuz87hke';
$DEFAULT_PATH = '/Devices/'.$deviceId;

date_default_timezone_set('Asia/Kolkata');

$_devicestatus= array(
'Vessel Type' => $Vtype,
'Country' => $country,
'IMO Number' => $IMOnumber,
'MMSI Number' => $MMSInumber,
'Vessel Name' => $Vesselname,
'Owner Name' => $Ownername,
'Size' => $Size,
'GT' => $GT,
'Latitude' => $latitude + 0.0,
'Longitude' => $longitude + 0.0,
'Distance from Coast' => $Distancefromcoast,
'Speed' => $Speed,
'Course' => $Course,
'Status' => $Status,
'Tamperproof' => $Tamperproof,
'Power Status' => $backup,
'Emergency' => $emerg,
'LastUpdated' => date('Y-m-d H:i:s')
);

 
$firebase = new fireBase(DEFAULT_URL, DEFAULT_TOKEN);
$firebase->update($DEFAULT_PATH, $_devicestatus); // updates data in Firebase

print("Cool Jarvis!!");
?>