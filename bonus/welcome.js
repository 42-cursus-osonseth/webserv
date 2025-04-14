document.addEventListener("DOMContentLoaded", function () {
    function getCookieValue(name) {
        const value = `; ${document.cookie}`;
        const parts = value.split(`; ${name}=`);
        if (parts.length === 2) return parts.pop().split(';').shift();
        return null;
    }

    function insertWelcomeMessage(afterElement) {
        const prenom = getCookieValue("Prenom");
        const nom = getCookieValue("Nom");

        if (prenom && nom && afterElement) {
            const welcomeDiv = document.createElement("div");
            welcomeDiv.id = "welcome-message";
            welcomeDiv.textContent = `Bienvenue ${prenom} ${nom} !`;
            welcomeDiv.style.textAlign = "center";
            welcomeDiv.style.margin = "20px 0";
            afterElement.insertAdjacentElement("afterend", welcomeDiv);
        }
    }

    function tryInsertMessage() {
        const nav = document.querySelector(".nav-bar");
        const header = document.querySelector("header");

        if (nav) {
            insertWelcomeMessage(nav);
        } else if (header) {
            setTimeout(() => {
                const navRetry = document.querySelector(".nav-bar");
                if (navRetry) {
                    insertWelcomeMessage(navRetry);
                } else {
                    insertWelcomeMessage(header);
                }
            }, 30);
        }
    }

    tryInsertMessage();
});
