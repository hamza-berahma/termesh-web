document.addEventListener("DOMContentLoaded", () => {
    const e = document.getElementById("tab-preset"),
        t = document.getElementById("tab-custom"),
        d = document.getElementById("panel-preset"),
        a = document.getElementById("panel-custom");
    if (e && t && d && a) {
        const s = [e, t],
            n = [d, a],
            r = (e, t) => {
                s.forEach((e) => {
                    (e.ariaSelected = "false"),
                        e.classList.remove("border-white", "text-white"),
                        e.classList.add("border-transparent", "text-gray-400");
                }),
                    n.forEach((e) => {
                        e.classList.add("hidden");
                    }),
                    (e.ariaSelected = "true"),
                    e.classList.add("border-white", "text-white"),
                    e.classList.remove("border-transparent", "text-gray-400"),
                    t.classList.remove("hidden");
            };
        e.addEventListener("click", () => r(e, d)),
            t.addEventListener("click", () => r(t, a)),
            r(e, d);
    }
});
