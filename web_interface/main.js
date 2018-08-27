new Vue({
    el: '#app',
    data: {
        devices: null,
        config: {}
    },
    methods: {
        getData: function () {
            var self = this;
            fetch("devices.json").then(response => {
                return response.json();
            }).then(data => {
                self.devices = data["devices"];
                var loader = document.getElementById("preloader");
                loader.classList.remove("active");
                loader.classList.add("hide");
            });
            /*fetch("config.json").then(response => {
                return response.json();
            }).then(data => {
                self.config = data;
            });*/
        },
        click: function (id) {
            // var device = this.devices.find(x => x.button === id);
            var loader = document.getElementById(id);
            loader.classList.remove("hide");
            fetch("command?action=click&button=" + id).then(response => {
                if (response.text() !== "OK") {
                    // boh
                }
                loader.classList.add("hide");
            });
        },
        /*saveSettings: function()
        {
            this.config.sinric_enabled = document.getElementById("sinric_enabled").checked;
            this.config.sinric_api_key = document.getElementById("sinric_api_key").value;

            var configString = JSON.stringify(this.config);

            fetch("command?action=updateSettings&settings=" + configString);
        }*/
    },
    created: function () {
        /*document.addEventListener('DOMContentLoaded', function () {
            var elems = document.querySelectorAll('.modal');
            var instances = M.Modal.init(elems);
        });*/

        this.getData();
    }
});