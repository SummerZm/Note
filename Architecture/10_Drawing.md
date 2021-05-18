## **怎么设计一个“画图”程序？**
> [相关链接](https://time.geekbang.org/column/article/108887)

### **Model层设计**
> **能直观体现业务**
- 1. 添加图形（Shape），可以是 QLine，QRect，QEllipse，QPath 等等。
- 2. 绘制（onpaint）
    ```sh
    # 方案1：为了 View 层能够绘制，需要让 DOM 层把自己的数据暴露给 View 层。
    # 方案2：从简洁的方式来说，是让 Model 层自己来绘制，这样就避免暴露 DOM 层的实现细节。
    # 取舍：
    #   a. 耦合 GDI 比暴露 DOM 的数据细节要好，因为 GDI 的接口通常来说更稳定。
    #   b. 在依赖选择上，我们会更倾向于依赖接口更为稳定的组件，因为这意味着我们的接口也更稳定。
    ```
- 3. 示例代码
    ```js
    class QLineStyle {
    properties:
    width: number
    color: string
    methods:
    constructor(width: number, color: string)
    }

    class QRect {
    properties:
    x, y, width, height: number
    lineStyle: QLineStyle
    methods:
    constructor(r: Rect, lineStyle: QLineStyle)
    onpaint(ctx: CanvasRenderingContext2D): void
    }

    interface Shape {
    onpaint(ctx: CanvasRenderingContext2D): void
    }

    class QPaintDoc {
    methods:
    addShape(shape: Shape): void
    onpaint(ctx: CanvasRenderingContext2D): void
    }
    ```

### **View and ViewModel层设计**
> **View 要做的工作一般已浏览器完成**

- ViewModel 层相关内容细节
    1. ViewModel 层主要是一个 index.htm 文件和一个 view.js 文件
        ```sh
        # index.htm 是总控文件
        #   主要包含两个东西：界面布局（Layout）；应用初始化（InitApplication），比如加载哪些 Controllers。
        ```
    2. 屏蔽平台的差异：
        ```sh
        # A. Model 层很容易做到平台无关，除了 GDI 会略微费劲一点；
        # B. Controller 层除了有少量的界面需要处理平台差异外，大部分代码都是响应事件处理业务逻辑
        # C. View 层对事件的抽象得当，也是跨平台的。
        ```
    3. 定义界面布局：不同尺寸的设备，界面交互也会不太一样，在 View 层来控制不同设备的整体界面布局比较妥当。

    4. 相关代码细节
        ```js
        interface Controller {
            stop(): void
            onpaint(ctx: CanvasRenderingContext2D): void
        }

        class QPaintView {
            properties:
                doc: QPaintDoc
            properties: {
                lineWidth: number
                lineColor: string
            }
            drawing: DOMElement
            controllers: map[string]Controller
            methods:
                get currentKey: string
                get lineStyle: QLineStyle
                onpaint(ctx: CanvasRenderingContext2D): void
                invalidateRect(rect: Rect): void
                registerController(name: string, controller: Controller): void
                invokeController(name: string): void
                stopController(): void
                getMousePos(event: DOMEvent): Point
            events:
                /* 
                    1. Web 开发一个很重要的优势是不用自己处理局部更新问题，为什么这里我们却又要自己处理呢？
                    2. 原因是当前没用浏览器的 Virtual View，整个 DOM 的数据组织完全自己管理，这样我们面临的问题就和传统桌面开发完全一致。
                    3. 所以此处的OnXXXX 主要是响应成员 当前活动的controllers: map[string]Controller 的绘制
                */
                onmousedown: (event: DOMEvent):void
                onmousemove: (event: DOMEvent):void
                onmouseup: (event: DOMEvent):void
                ondblclick: (event: DOMEvent):void
                onkeydown: (event: DOMEvent):void
        }

        var qview = new QPaintView()
        ```

- View 层相关内容细节
    1. View 层并不关心具体的 Controller 都有些什么，但是会对它们的行为规则进行定义。[各种onXxxx()事件]
    2. 事件委托（delegate）：允许 Controller 选择自己感兴趣的事件进行响应。[controller实现的onXxxx()事件]

### **Controller层设计**

- **避免了两类 Controller 相互耦合**
1. 菜单并不直接和各类创建图形的 Controller 打交道，而是调用 qview.invokeController 来激活对应的 Controller，

- **同一个 DOM API 在 Controller 层往往会有多条实现路径**
1. Model 层支持的图形只有 QLine、QRect、QEllipse、QPath 等四种，
2. 但是界面表现有六种：Line、Rect、Ellipse、Circle、Path、FreePath 等等。这是非常正常的现象。

- **相关代码设计：**
    ```js
    class QPathCreator {
        /* view 是 viewModel中的qview对象 */
        constructor(view, close) {
            this.points = []
            this.close = close
            this.fromPos = this.toPos = {x: 0, y: 0}
            this.started = false
            this.view = view
            let ctrl = this
            view.onmousedown = function(event) { ctrl.onmousedown(event) }
            view.onmousemove = function(event) { ctrl.onmousemove(event) }
            view.ondblclick = function(event) { ctrl.ondblclick(event) }
            view.onkeydown = function(event) { ctrl.onkeydown(event) }
        }
        // ....
    }
    ```

### **其他**
- 为了避免 Model 知道 View 的实现细节，可以让 Model 耦合 GDI 接口。
- 模块间通信如果避免不了耦合，就耦合稳定的模块，这个模块最好是系统的，因为系统模块相对于业务模块通常更加稳定；
- ViewModel 持有 Model，并由 Controller 来更新 Model/ViewModel；
- ViewModel 定义 Controller 的行为规则，但并不关心 Controller 的具体行为。Controller 可以选择性的接管 ViewModel 的事件；
- ViewModel 协调 Model 和 Controller，启到承上启下的作用，所以 ViewModel 职责的划分对程序的结构有比较大的影响；
- 避免 Controller 之间的耦合，可以使用 ViewModel 作为通信中介者；
- 相同的 Model 可能在 Controller 层有不同的展现方法；
























































