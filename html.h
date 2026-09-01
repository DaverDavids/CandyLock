#pragma once
#include <Arduino.h>

String buildHTML(
  const String &channel,
  const String &oauth,
  const String &botname,
  bool tCmd, bool tAny, bool tVIP, bool tSub, bool tMod, bool tBroad,
  const String &cmds,
  const String &whitelist,
  const String &blacklist,
  bool debug
) {
  auto chk = [](bool b) -> String { return b ? " checked" : ""; };
  String locked = debug ? "" : " disabled";

  String html = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>CandyLock</title>
<style>
  body{font-family:sans-serif;background:#0e0e10;color:#efeff1;max-width:520px;margin:30px auto;padding:0 16px}
  h1{color:#9147ff;margin-bottom:4px}h2{color:#bf94ff;font-size:1em;margin:20px 0 6px}
  input[type=text],input[type=password],textarea{
    width:100%;box-sizing:border-box;background:#18181b;color:#efeff1;
    border:1px solid #3a3a3d;border-radius:4px;padding:7px;font-size:.95em;margin-top:3px}
  textarea{height:68px;resize:vertical}
  label{display:flex;align-items:center;gap:8px;margin:6px 0;cursor:pointer}
  input[type=checkbox]{width:18px;height:18px;accent-color:#9147ff}
  button{background:#9147ff;color:#fff;border:none;border-radius:4px;padding:10px 28px;
    font-size:1em;cursor:pointer;margin-top:14px;width:100%}
  button:hover{background:#7c3aed}
  .note{font-size:.8em;color:#adadb8;margin-top:3px}
  hr{border:none;border-top:1px solid #3a3a3d;margin:18px 0}
  /* Status dashboard */
  .status{display:flex;gap:10px;flex-wrap:wrap;margin:10px 0}
  .pill{flex:1;min-width:100px;background:#18181b;border:1px solid #3a3a3d;
    border-radius:8px;padding:10px 12px;text-align:center}
  .pill .k{font-size:.75em;text-transform:uppercase;letter-spacing:.05em;color:#adadb8}
  .pill .v{font-size:1.1em;font-weight:bold;margin-top:4px}
  .ok{color:#5cb85c}.no{color:#e05252}.on{color:#9147ff}
  .solenoid{background:#9147ff;border-color:#9147ff}
  .solenoid .v{color:#fff}
  #chat{background:#18181b;border:1px solid #3a3a3d;border-radius:8px;
    max-height:300px;overflow-y:auto;padding:6px 8px;font-size:.92em;margin-top:8px}
  .chatline{padding:3px 6px;border-radius:4px;white-space:pre-wrap;word-break:break-word}
  .chatline.trigger{background:#9147ff;color:#fff;font-weight:bold;box-shadow:inset 3px 0 0 #7c3aed}
  .chatline .n{font-weight:bold;color:#bf94ff;margin-right:6px}
  .chatline.trigger .n{color:#fff}
  .chatline.empty{color:#5a5a5f;text-align:center;font-style:italic}
  .banner{background:#18181b;border:1px solid #3a3a3d;border-radius:8px;
    padding:8px 12px;font-size:.85em;color:#adadb8;margin:10px 0}
</style>
</head>
<body>
<h1>&#x1F512; CandyLock</h1>
<p class="note">Live status &amp; chat. Unlock is triggered only by qualifying chat messages.</p>
)rawhtml";
  if (!debug) {
    html += "<div class='banner'>&#x1F512; <b>Settings locked.</b> Edit mode is disabled because debug mode is off. Enable <code>ENABLE_DEBUG</code> at the top of the sketch to change settings.</div>";
  }
  html += R"rawhtml(
<h2>Status</h2>
<div class="status" id="status"></div>
)rawhtml";
  if (debug) {
    html += "<button type='button' id='trigbtn' onclick='manualTrigger()' style='background:#7c3aed'>&#x26A1; Manual Unlock (debug)</button>";
    html += "<button type='button' id='ledbtn' onclick='manualLedTest()' style='background:#2f855a;margin-top:8px'>&#x2B50; GPIO0 LED Test (debug)</button>";
  }
  html += R"rawhtml(
<h2>Chat</h2>
<div id="chat"><div class="chatline empty">Connecting&#8230;</div></div>
<p class="note">Lines that triggered the unlock are highlighted purple. Last 30 messages.</p>

<hr>
<h2>Twitch Channel</h2>
<form method="POST" action="/save">
  <fieldset>)rawhtml";
  html += locked;
  html += R"rawhtml(>
  <input type="text" name="channel" placeholder="channelname (no #)" value=")rawhtml";
  html += channel;
  html += R"rawhtml("><br>

  <h2>IRC Credentials</h2>
  <input type="text"     name="botname" placeholder="Bot username (blank = anonymous)" value=")rawhtml";
  html += botname;
  html += R"rawhtml("><br>
  <input type="password" name="oauth"   placeholder="oauth:xxxxx (blank = anonymous)" value=")rawhtml";
  html += oauth;
  html += R"rawhtml("><br>
  <p class="note">Leave both blank to join anonymously (read-only, no chat send).</p>

  <hr>
  <h2>Who Can Trigger</h2>
  <label><input type="checkbox" name="tBroad")rawhtml";
  html += chk(tBroad);
  html += R"rawhtml(> Broadcaster</label>
  <label><input type="checkbox" name="tMod")rawhtml";
  html += chk(tMod);
  html += R"rawhtml(> Moderators</label>
  <label><input type="checkbox" name="tVIP")rawhtml";
  html += chk(tVIP);
  html += R"rawhtml(> VIPs</label>
  <label><input type="checkbox" name="tSub")rawhtml";
  html += chk(tSub);
  html += R"rawhtml(> Subscribers</label>
  <label><input type="checkbox" name="tAny")rawhtml";
  html += chk(tAny);
  html += R"rawhtml(> Any chatter</label>

  <p class="note">Whitelisted users below always qualify regardless of badges.</p>

  <hr>
  <h2>Trigger Commands</h2>
  <label><input type="checkbox" name="tCmd")rawhtml";
  html += chk(tCmd);
  html += R"rawhtml(> Require message to match a command below</label>
  <textarea name="cmds" placeholder="!lock, !open, !trigger">)rawhtml";
  html += cmds;
  html += R"rawhtml(</textarea>
  <p class="note">Comma-separated. If unchecked, any message from a qualified user triggers.</p>

  <hr>
  <h2>Whitelist <span style="font-weight:normal;color:#adadb8">(always allowed)</span></h2>
  <textarea name="whitelist" placeholder="user1, user2">)rawhtml";
  html += whitelist;
  html += R"rawhtml(</textarea>

  <h2>Blacklist <span style="font-weight:normal;color:#adadb8">(never triggers)</span></h2>
  <textarea name="blacklist" placeholder="spammer1, bot2">)rawhtml";
  html += blacklist;
  html += R"rawhtml(</textarea>

  <button type="submit">&#x1F4BE; Save &amp; Apply</button>
  </fieldset>
</form>

<hr>
<h2>WiFi Credentials</h2>
<form method="POST" action="/wifi">
  <fieldset>)rawhtml";
  html += locked;
  html += R"rawhtml(>
  <input type="text"     name="ssid" placeholder="SSID"><br>
  <input type="password" name="psk"  placeholder="Password"><br>
  <button type="submit">Save WiFi &amp; Reboot</button>
  </fieldset>
</form>

<script>
function e(name, cls, label, value){
  return '<div class="pill '+(cls||'')+'"><div class="k">'+label+'</div>'+
         '<div class="v '+(cls||'')+'">'+value+'</div></div>';
}
function el(tag, cls, html){
  var d=document.createElement(tag); if(cls) d.className=cls; d.innerHTML=html; return d;
}
function manualTrigger(){
  fetch('/trigger', {method:'POST'}).then(function(r){
    var b=document.getElementById('trigbtn');
    if(b){ b.innerHTML = r.ok? '&#x26A1; Unlocked!' : '&#x26A1; Denied'; }
    setTimeout(function(){ if(b) b.innerHTML='&#x26A1; Manual Unlock (debug)'; }, 1500);
  });
}
function manualLedTest(){
  fetch('/ledtest', {method:'POST'}).then(function(r){
    var b=document.getElementById('ledbtn');
    if(b){ b.innerHTML = r.ok? '&#x2B50; Lit!' : '&#x2B50; Denied'; }
    setTimeout(function(){ if(b) b.innerHTML='&#x2B50; GPIO0 LED Test (debug)'; }, 1500);
  });
}
function poll(){
  fetch('/status').then(function(r){return r.json();}).then(function(s){
    var wifi = s.wifi? '<span class="ok">Connected</span><br><small>'+esc(s.ssid)+' '+esc(s.ip)+'</small>'
                      : '<span class="no">Disconnected</span>';
    var irc  = s.irc? '<span class="ok">Connected</span><br><small>#'+esc(s.channel)+'</small>'
                      : '<span class="no">Disconnected</span>';
    var sol  = s.solenoid? '<span class="on">ACTIVE</span>' : '<span>Idle</span>';
    document.getElementById('status').innerHTML =
      e('','','WiFi',wifi) + e('','','Twitch',irc) +
      e(s.solenoid?'solenoid':'','','Solenoid',sol);

    var chat = document.getElementById('chat');
    chat.innerHTML='';
    if(!s.chat || s.chat.length===0){
      chat.appendChild(el('div','chatline empty','No messages yet'));
    } else {
      s.chat.forEach(function(c){
        var d = el('div', 'chatline'+(c.t?' trigger':''));
        var nm = document.createElement('span');
        nm.className = 'n';
        nm.textContent = String(c.n || '') + ': ';
        d.appendChild(nm);
        d.appendChild(document.createTextNode(String(c.m || '')));
        chat.appendChild(d);
      });
      chat.scrollTop = chat.scrollHeight;
    }
  }).catch(function(){});
}
function esc(s){
  return s.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;')
          .replace(/"/g,'&quot;');
}
setInterval(poll, 1500);
poll();
</script>
</body>
</html>
)rawhtml";
  return html;
}
