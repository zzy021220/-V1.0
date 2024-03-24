
#include "genki_web_ohosdog.h"

#include <stdio.h>
#include "utils_file.h"
#include <stdint.h>
#include <stdlib.h>
#include "dog.h"

static void save_html(const char *page, const char *html) {
    int fd = UtilsFileOpen(page, O_CREAT_FS | O_TRUNC_FS | O_WRONLY_FS, 0);

    if (fd > 0) {
        UtilsFileWrite(fd, html, strlen(html));
        UtilsFileClose(fd);
    }
}

static int doHtml(genki_web_request_t *request, genki_web_response_t *response) {
    if (request->method != GET) {
        return -1;
    }
    const char *page = "plot.html";
    unsigned int size;
    char buf[128];
    if (UtilsFileStat(page, &size) >= 0) {
        sprintf(buf, "%d", size);
        response->setHeader(response, "Content-Length", buf);

        int fd = UtilsFileOpen(page, O_RDONLY_FS, 0);
        if (fd > 0) {
            unsigned char num;
            while ((num = UtilsFileRead(fd, buf, 128)) > 0) {
                response->write(response, buf, num);
            }
            UtilsFileClose(fd);
        } else {
            sprintf(buf, "<html><body><h2 id='a'>ERROR</h2></body></html>");
            response->write(response, buf, strlen(buf));
        }
    } else {
        const char *html = \
        "<!DOCTYPE html>\
        <html lang=\"en\">\
            <head>\
                <meta charset=\"UTF-8\">\
                <title>元气派</title>\
                <style>\
                    body {user-select: none;text-align: center;}\
                    table {margin: 0 auto}\
                    td div {\
                        width: 150px;\
                        height: 150px;\
                        border: 1px solid darkgrey;\
                        display: flex;\
                        align-items: center;\
                        justify-content: center;\
                        color: black;\
                    }  \
                    #s {\
                        background-color: red;\
                        color: white;\
                    }  \
                    td div:active {\
                        background: deepskyblue;\
                        color: white;\
                    }  \
                    button {\
                        margin: 0.5rem;\
                        width: 9rem;\
                        height: 3rem;\
                        font-size: 1.2rem;\
                        border-radius: 1rem;\
                    }  \
                    button:hover {\
                        border-radius: 1rem;\
                        background-color: deepskyblue;\
                        color: aliceblue;\
                    }\
                </style>\
            </head>\
            <body>\
                <h1>鸿蒙狗控制</h1>\
                <button id=\"a\">初始化动作</button>\
                <br>\
                <br>\
                <button id=\"b\">前进</button>\
                <br>\
                <br>\
                <button id=\"d\">左转</button>\
                <button id=\"e\">右转</button>\
                <br>\
                <br>\
                <button id=\"c\">后退</button>\
                <br>\
                <br>\
                <button id=\"f\">握手</button>\
                <br>\
                <br>\
                <button id=\"g\">扭身子</button>\
                <br>\
                <br>\
                <button id=\"h\">伸懒腰</button>\
                <br>\
                <br>\
                <button id=\"i\">站立</button>\
                <button id=\"j\">趴下</button>\
                <br>\
                <br>\
                <button id=\"k\">动作组合1</button>\
                <br>\
                <br>\
                <button id=\"l\">动作组合2</button>\
                <br>\
                <br>\
                <script> \
                    function u(p) {\
                        return `${p}`;\
                    }\
                    function g(i) {\
                        return document.getElementById(i);\
                    }\
                    function fe(e, f) {\
                        e.addEventListener('click', function (e) {\
                            /* do something*/\
                            f();\
                        });\
                    }\
                    function fg(u, f) {\
                        let x = new XMLHttpRequest();\
                        x.onreadystatechange = function () {\
                            if (this.readyState == 4 && this.status == 200) {\
                                f(x);\
                            }\
                        }\
                        ;\
                        x.open(\"GET\", u, true);\
                        x.send();\
                    }\
                    function fp(u, d, f, p) {\
                        let x = new XMLHttpRequest();\
                        if (p) {\
                            x.upload.addEventListener('progress', function (e) {\
                                p(e);\
                            });\
                        }\
                        x.onreadystatechange = function () {\
                            if (this.readyState == 4 && this.status == 200) {\
                                f(x);\
                            }\
                        }\
                        ;\
                        x.open('POST', u, true);\
                        x.timeout = 45000;\
                        x.send(d);\
                    }\
                    fe(g('a'), () => {\
                        fg(u('/dog/init'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('b'), () => {\
                        fg(u('/dog/forward'), () => {}\
                        );\
                    }\
                    );\
                   fe(g('c'), () => {\
                        fg(u('/dog/backward'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('d'), () => {\
                        fg(u('/dog/leftturn'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('e'), () => {\
                        fg(u('/dog/rightturn'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('f'), () => {\
                        fg(u('/dog/wink'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('g'), () => {\
                        fg(u('/dog/twist'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('h'), () => {\
                        fg(u('/dog/shof'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('i'), () => {\
                        fg(u('/dog/standhome'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('j'), () => {\
                        fg(u('/dog/sithome'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('k'), () => {\
                        fg(u('/dog/dostan1'), () => {}\
                        );\
                    }\
                    );\
                    fe(g('l'), () => {\
                        fg(u('/dog/dostan2'), () => {}\
                        );\
                    }\
                    );\
                </script>\
            </body>\
        </html>";
        size_t len = strlen(html);

        char buf[128];
        sprintf(buf, "%d", len);
        response->setHeader(response, "Content-Type", "text/html; charset=UTF-8");
        response->setHeader(response, "Content-Length", buf);
        response->write(response, html, len);
        save_html(page, html);
    }

    return 1;
}

// static int dotest(genki_web_request_t *request, genki_web_response_t *response) {
//     test();       //测试

//     return 1;
// }

static int doInit(genki_web_request_t *request, genki_web_response_t *response) {
    initial();       //初始化动作

    return 1;
}

static int dostan1(genki_web_request_t *request, genki_web_response_t *response) {
     stand1();      //动作组合1

    return 1;
}

static int dostan2(genki_web_request_t *request, genki_web_response_t *response) {
    stand2();       //動作2

    return 1;
}

static int doshof(genki_web_request_t *request, genki_web_response_t *response) {
    stand3();       //  伸懒腰

    return 1;
}

static int dodownaction(genki_web_request_t *request, genki_web_response_t *response) {
    downaction();

    return 1;
}

static int dowink(genki_web_request_t *request, genki_web_response_t *response) {
    wink(5);        //握手

    return 1;
}

static int dotwist(genki_web_request_t *request, genki_web_response_t *response) {
    twist(5);       //扭腰

    return 1;
}

static int doforward(genki_web_request_t *request, genki_web_response_t *response) {
    forward(5);     //前进

    return 1;
}

static int dobackward(genki_web_request_t *request, genki_web_response_t *response) {
    backward(5);        //后退

    return 1;
}

static int dorightturn(genki_web_request_t *request, genki_web_response_t *response) {
    rightturn(5);       //右转

    return 1;
}

static int doleftturn(genki_web_request_t *request, genki_web_response_t *response) {
    leftturn(5);        //左转

    return 1;
}

static int dostandhome(genki_web_request_t *request, genki_web_response_t *response) {
    standhome();        //站立

    return 1;
}

static int dosithome(genki_web_request_t *request, genki_web_response_t *response) {
    sithome();      //趴下

    return 1;
}

static int doCmds(genki_web_request_t *request, genki_web_response_t *response) {
    if (request->method == GET) return -1;

    int contentLength = 0;
    char *cl = request->getHeader(request, "Content-Length");
    if (cl == NULL) {
        return -1;
    }
    contentLength = atoi(cl);
    char buf[2048];
    int num;
    num = request->read(request, buf, 2048);

    dog_execJson(buf, num);

    return 1;
}


genki_web_service_t *newDogService(void) {
    genki_web_service_t *service = genki_web_newService("DOG");
    service->addFilter(service, "/dog", doHtml);
    service->addFilter(service, "/dog/init", doInit);
    service->addFilter(service, "/dog/forward", doforward);
    service->addFilter(service, "/dog/backward", dobackward);
    service->addFilter(service, "/dog/leftturn", doleftturn);
    service->addFilter(service, "/dog/rightturn", dorightturn);
    service->addFilter(service, "/dog/wink", dowink);
    service->addFilter(service, "/dog/twist", dotwist);
    service->addFilter(service, "/dog/shof", doshof);
    service->addFilter(service, "/dog/dostan1", dostan1);
    service->addFilter(service, "/dog/dostan2", dostan2);
    service->addFilter(service, "/dog/standhome", dostandhome);
    service->addFilter(service, "/dog/sithome", dosithome);

    service->link_name = "Dog控制";
    service->link_url = "/dog";
    return service;
}