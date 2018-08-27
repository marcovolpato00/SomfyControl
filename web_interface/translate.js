var langJson = {};

function translate() {
	fetch("translation.lang").then(response => {
		return response.json();
	}).then(data => {
		parseLang(data);
	});
}

function getTranslation(key) {
	return convertLineBreaks(esc(langJson[key]));
}

function parseLang(data) {
	langJson = data;
	var elements = document.querySelectorAll("[translate-key]");
	for (i = 0; i < elements.length; i++) {
		var element = elements[i];
		element.innerHTML = getTranslation(element.getAttribute("translate-key"));
	}
}

function convertLineBreaks(str) {
	if (str) {
		str = str.toString();
		str = str.replace(/(?:\r\n|\r|\n)/g, '<br>');
		return str;
	}
	return "";
}

function esc(str) {
	if (str) {
		return str.toString()
			.replace(/&/g, '&amp;')
			.replace(/</g, '&lt;')
			.replace(/>/g, '&gt;')
			.replace(/\"/g, '&quot;')
			.replace(/\'/g, '&#39;')
			.replace(/\//g, '&#x2F;');
	}
	return "";
}
