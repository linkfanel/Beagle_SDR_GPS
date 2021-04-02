<?php

include "../../php/common.php";

if (server('REQUEST_METHOD') == 'GET' && request('key') == '4e92a0c3194c62b2a067c494e2473e8dfe261138') {

$IP_PUB = server('REMOTE_ADDR');
//elog('XDLS: IP_PUB='. $IP_PUB);

// NB: heredoc so "$IP_PUB" gets substituted below
$html = <<<EOF
<!DOCTYPE html><meta charset=utf-8><script>var kiwi_check_js_version=[],xdLocalStorage_ip="$IP_PUB";window.XdUtils=window.XdUtils||function(){function e(e,t){var a,n=t||{};for(a in e)e.hasOwnProperty(a)&&(n[a]=e[a]);return n}return{extend:e}}(),function(){function e(e,t){var a=XdUtils.extend(t,d);a.etag=localStorage.getItem("etag"),a.id=e,parent.postMessage(JSON.stringify(a),"*")}function t(t,a){e(t,{key:a,value:localStorage.getItem(a)})}function a(t,a,n,o){localStorage.setItem(n,o),localStorage.setItem("etag",t),e(a,{success:localStorage.getItem(n)===o})}function n(t,a,n){localStorage.removeItem(n),localStorage.setItem("etag",t),e(a,{})}function o(t,a){e(t,{key:localStorage.key(a)})}function i(t){e(t,{size:JSON.stringify(localStorage).length})}function c(t){e(t,{length:localStorage.length})}function r(t,a){localStorage.clear(),localStorage.setItem("etag",t),e(a,{})}function s(t){e(t,{})}function g(e){var g;try{g=JSON.parse(e.data)}catch(e){}g&&g.namespace===l&&(d.action=g.action,d.server=g.server,"set"===g.action?a(g.etag,g.id,g.key,g.value):"get"===g.action?t(g.id,g.key):"remove"===g.action?n(g.etag,g.id,g.key):"key"===g.action?o(g.id,g.key):"size"===g.action?i(g.id):"length"===g.action?c(g.id):"clear"===g.action?r(g.etag,g.id):"ping"===g.action&&s(g.id))}var l="cross-domain-local-message",d={namespace:l};window.addEventListener?window.addEventListener("message",g,!1):window.attachEvent("onmessage",g),function(){var e={namespace:l,id:"iframe-ready",ip:xdLocalStorage_ip};parent.postMessage(JSON.stringify(e),"*")}()}()</script>
EOF;

echo $html;

} else {
    http_response_code(400);
    echo "400 - Bad Request";
}

?>
