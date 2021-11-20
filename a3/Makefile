out_dir = build
export

.PHONY: all
all:
	mkdir -p $(out_dir)
	cmake -B$(out_dir) -H.
	$(MAKE) -C$(out_dir) -s
	ln -sf $(out_dir)/graph_driver graph_driver

.PHONY: clean
clean:
	rm -r $(out_dir)
	rm graph_driver
