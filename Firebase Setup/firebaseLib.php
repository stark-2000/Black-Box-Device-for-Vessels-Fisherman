<?php
class Firebase
{
    private $_baseURI;
    private $_timeout;
    private $_token;

    function __construct($baseURI = 'https://ferry-smart.firebaseio.com/', $token = 'EAwWIIIOJOnRgmuTxO5AIuwMAMPyRk7DUuz87hke')
    {
        if (!extension_loaded('curl')) {
            trigger_error('Extension CURL is not loaded.', E_USER_ERROR);
        }

        $this->setBaseURI($baseURI);
        $this->setTimeOut(10);
        $this->setToken($token);
    }

    public function setToken($token)
    {
        $this->_token = $token;
    }

    public function setBaseURI($baseURI)
    {
        $baseURI .= (substr($baseURI, -1) == '/' ? '' : '/');
        $this->_baseURI = $baseURI;
    }

    private function _getJsonPath($path)
    {
        $url = $this->_baseURI;
        $path = ltrim($path, '/');
        $auth = ($this->_token == '') ? '' : '?auth=' . $this->_token;
        return $url . $path . '.json' . $auth;
    }

    public function setTimeOut($seconds)
    {
        $this->_timeout = $seconds;
    }

    public function set($path, $data)
    {
      return $this->_writeData($path, $data, 'PUT');
    }

    public function push($path, $data)
    {
      return $this->_writeData($path, $data, 'POST');
    }

    public function update($path, $data)
    {
      return $this->_writeData($path, $data, 'PATCH');
    }

    public function get($path)
    {
        try {
            $ch = $this->_getCurlHandler($path, 'GET');
            $return = curl_exec($ch);
            curl_close($ch);
        } catch (Exception $e) {
            $return = null;
        }
        return $return;
    }

    public function delete($path)
    {
        try {
            $ch = $this->_getCurlHandler($path, 'DELETE');
            $return = curl_exec($ch);
            curl_close($ch);
        } catch (Exception $e) {
            $return = null;
        }
        return $return;
    }

    private function _getCurlHandler($path, $mode)
    {
        $url = $this->_getJsonPath($path);
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_TIMEOUT, $this->_timeout);
        curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $this->_timeout);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $mode);
        return $ch;
    }

    private function _writeData($path, $data, $method = 'PUT')
    {
        $jsonData = json_encode($data);
        $header = array(
            'Content-Type: application/json',
            'Content-Length: ' . strlen($jsonData)
        );
        try {
            $ch = $this->_getCurlHandler($path, $method);
            curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
            curl_setopt($ch, CURLOPT_POSTFIELDS, $jsonData);
            $return = curl_exec($ch);
            curl_close($ch);
        } catch (Exception $e) {
            $return = null;
        }
        return $return;
    }

}