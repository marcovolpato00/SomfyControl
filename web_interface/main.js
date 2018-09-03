new Vue({
    el: '#app',
    data: {
        devices: null,
        config: {},
        langs: null
    },
    methods: {
        getDevices: function () {
            var self = this;
            fetch("devices.json", { credentials: "same-origin" }).then(response => {
                return response.json();
            }).then(data => {
                self.devices = data["devices"];
                var loader = document.getElementById("preloader");
                loader.classList.remove("active");
                loader.classList.add("hide");
            });
        },
        getConfig: function () {
            var self = this;
            fetch("config.json", { credentials: "same-origin" }).then(response => {
                return response.json();
            }).then(data => {
                self.config = data;
                self.langs = self.config.languages;
                translate(self.config.language);
            });
        },
        click: function (id) {
            // var device = this.devices.find(x => x.button === id);
            var loader = document.getElementById(id);
            loader.classList.remove("hide");
            fetch("command?action=click&button=" + id, { credentials: "same-origin" }).then(response => {
                if (response.text() !== "OK") {
                    // boh
                }
                loader.classList.add("hide");
            });
        },
        saveSettings: function () {
            this.config.sinric_enabled = document.getElementById("sinric_enabled").checked;

            var keys = ["sinric_api_key", "web_username", "web_password", "ssid", "password", "language"];
            var self = this;
            keys.forEach(function (key) {
                self.config[key] = document.getElementById(key).value;
            });

            var configString = JSON.stringify(this.config);

            console.log(configString);
            fetch("command?action=updateSettings", {
                method: 'POST',
                headers: {
                    'Accept': 'application/json',
                    'Content-Type': 'application/json'
                },
                credentials: "same-origin",
                body: configString
            });
            location.href = "/login.html";
        },
        showPasswords: function () {
            var pass = document.querySelectorAll('[placeholder="Password"]');
            pass.forEach(function (elem) {
                if (elem.getAttribute("type") == "text") {
                    elem.setAttribute("type", "password");
                }
                else {
                    elem.setAttribute("type", "text");
                }
            });
        }
    },
    created: function () {
        var self = this;
        document.addEventListener('DOMContentLoaded', function () {
            var modal = document.querySelectorAll('.modal');
            M.Modal.init(modal, {
                dismissible: false,
                onCloseEnd: function () {
                    // trick for reloading all input fields
                    var obj = JSON.parse(JSON.stringify(self.config));
                    self.config = obj;
                }
            });

            var sidenav = document.querySelectorAll('.sidenav');
            M.Sidenav.init(sidenav);
        });

        this.getDevices();
        this.getConfig();
    }
});